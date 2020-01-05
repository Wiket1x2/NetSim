//
// Created by krzys on 10.12.2019.
//

#ifndef NET_SIM_NODES_HPP
#define NET_SIM_NODES_HPP

#include "package.hpp"
#include "storage_types.hpp"
#include <utility>
#include <list>
#include <map>
#include <memory>
#include <stdexcept>

//enum class ReceiverType {RAMP, WORKER, STOREHOUSE};


class IPackageReceiver {
public:
    using lstP_ci=std::list<Package>::const_iterator; //zalozenie konspektowe - wszystkie wezly korzystaja z kontenera list
    virtual void receive_package(Package&& p)=0;
//    virtual std::pair<ReceiverType,ElementID> get_id() const =0; to na razie nie
    virtual ElementID get_id() const =0;
    virtual lstP_ci cbegin() const = 0;
    virtual lstP_ci cend() const = 0;
    virtual lstP_ci begin() const = 0;
    virtual lstP_ci end() const = 0;
    virtual ~IPackageReceiver()= default; //klasa macierzysta
};


class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
    ReceiverPreferences() = default; //FIXME zapytaj o to
    ReceiverPreferences(ProbabilityGenerator pg): pg_(pg) {}
    const_iterator cbegin() const { return probability_map.cbegin(); }
    const_iterator cend() const { return probability_map.cend(); }
    const_iterator begin() const { return probability_map.cbegin(); }
    const_iterator end() const { return probability_map.cend(); }
    void add_receiver(IPackageReceiver* r); //FIXME jak dla mnie moze tu byc const (przyczyna ew. bledow) (R) ta funkcja zmienia nam stan obiektu wiec po co const?
    void add_receiver(IPackageReceiver* r, double probability); //w przyszlosci mozliwe faworyzowanie odbiorcy
    void remove_receiver(IPackageReceiver* r);//FIXME jak dla mnie moze tu byc const (przyczyna ew. bledow) (R) ta funkcja zmienia nam stan obiektu wiec po co const?
    IPackageReceiver* choose_receiver(); //tu wartosc zwracana const byc nie moze bo poznej modyfikowana w senderze

private:
    ProbabilityGenerator pg_;
    preferences_t probability_map; //kontenery standardowe sa domyslnie inicjalizowane wartoscia {{}}- pusta mapa???
};


class PackageSender {
public:
    PackageSender(ReceiverPreferences receiver_preferences): receiver_preferences_(receiver_preferences) {} //FIXME pytanie - czy receiver preferences powinno zostac przekazane jako (stala) referencja tu i wklasach Worker/Ramp?
    ReceiverPreferences receiver_preferences_;
    void send_package();
//    std::optional<Package> get_sending_buffer() const {return buffor_package_;} //TODO problem - jak zwrocic buffor_package_ POPYTAJ
    bool get_sending_buffer() const {return bool(buffor_package_);} //TODO tymczasowa pomoc do testów
    virtual ~PackageSender()= default;
protected:
    void push_package(Package&& package); //FIXME a co jesli bufor juz byl pelny? - poprawione ale oczekuje na odp (R) ok, można zabezpieczyc

private:
    std::optional<Package> buffor_package_{std::nullopt};
};


class Ramp : public PackageSender{
public:
    Ramp(ElementID id, TimeOffset di,ReceiverPreferences receiver_preferences);
    void deliver_goods(Time t); //od razu push package zapewnione
    TimeOffset get_delivery_interval() const { return di_; }
    ElementID get_id() const { return id_; }
    ~Ramp() override;

/*    Ramp(const Package&)= delete;
    Ramp(Ramp&& other): id_(other.id_)  { other.id_=0; }
    Ramp& operator=(const Ramp&)= delete;
    Ramp& operator=(Ramp&& other);
    ~Ramp();*/

private:
    static std::set<ElementID> Ramp_IDs;
    ElementID id_;
    TimeOffset di_;
};


class Worker : public PackageSender, public IPackageReceiver{
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q,ReceiverPreferences receiver_preferences);
    void do_work(Time t);
    TimeOffset get_processing_duration() const { return pd_; } //pd - processing duration
    Time get_package_processing_start_time() const { return start_t;}
    void receive_package (Package&& p) override { q_->push(std::move(p)); }// worker ma zawierac bufor do wysylki - ten od PS , swoja kolejke FIFO/LIFO i bufor aktualny
    ElementID get_id() const override { return id_; }
    std::size_t get_queue_size() const { return q_->size(); }  //FIXME dodana by test nie miał 50 linijek
    lstP_ci cbegin() const override {return q_->cbegin();} //czy o to chodzilo z metodami delegujacymi?
    lstP_ci cend() const override {return q_->cend();}
    lstP_ci begin() const override {return q_->cbegin();}
    lstP_ci end() const override {return q_->cend();}
    ~Worker() override;

/*    Worker(const Package&)= delete;
    Worker(Worker&& other): id_(other.id_)  { other.id_=0; }
    Worker& operator=(const Worker&)= delete;
    Worker& operator=(Worker&& other);*/

private:
    static std::set<ElementID> Worker_IDs;
    ElementID id_;
    TimeOffset pd_;
    Time start_t;
    std::unique_ptr<IPackageQueue> q_;
    std::optional<Package> processing_buffor_{std::nullopt};
};


class Storehouse: public IPackageReceiver{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d);
    void receive_package (Package&& p) override { d_->push(std::move(p)); }
    ElementID get_id() const override { return id_; }
    std::size_t get_queue_size() const { return d_->size(); }  //FIXME dodana by test nie miał 50 linijek
    lstP_ci cbegin() const override {return d_->cbegin();} //o to chodzilo z metodami delegujacymi (wywoluja te [nadpisane] z klasy IPackageStockpile)
    lstP_ci cend() const override {return d_->cend();}
    lstP_ci begin() const override {return d_->cbegin();}
    lstP_ci end() const override {return d_->cend();}
    ~Storehouse() override;

private:
    static std::set<ElementID> Storehouse_IDs;
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};


#endif //NET_SIM_NODES_HPP
