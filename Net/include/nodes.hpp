//
// Created by krzys on 10.12.2019.
//

#ifndef NET_SIM_NODES_HPP
#define NET_SIM_NODES_HPP

#include "package.hpp"
#include "storage_types.hpp"
#include "helpers.hpp"
#include <utility>
#include <list>
#include <map>
#include <memory>
#include <stdexcept>


enum class ReceiverType {WORKER, STOREHOUSE};


class IPackageReceiver {
public:
    virtual void receive_package(Package&& p)=0;
    virtual ElementID get_id() const =0;
//    virtual ReceiverType get_receiver_type() const =0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;
    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;
    virtual ~IPackageReceiver()= default; //klasa macierzysta
};


class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences(ProbabilityGenerator pg = probability_generator): pg_(pg) {}
    const preferences_t& get_preferences() const {return probability_map;}
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver();
    const_iterator cbegin() const { return probability_map.cbegin(); }
    const_iterator cend() const { return probability_map.cend(); }
    const_iterator begin() const { return probability_map.cbegin(); }
    const_iterator end() const { return probability_map.cend(); }

    ~ReceiverPreferences() = default;
    ReceiverPreferences(const ReceiverPreferences&) = delete;
    ReceiverPreferences& operator = (const ReceiverPreferences&) = delete;
    ReceiverPreferences(ReceiverPreferences&&) = default;
    ReceiverPreferences& operator = (ReceiverPreferences&&) = default;
private:
    ProbabilityGenerator pg_;
    preferences_t probability_map;
};


class PackageSender {
public:
    void send_package();
    const std::optional<Package>& get_sending_buffer() const {return buffor_package_;}
    virtual ~PackageSender()= default;

    PackageSender() = default;
    PackageSender(const PackageSender&) = delete;
    PackageSender& operator = (const PackageSender&) = delete;
    PackageSender(PackageSender&&) = default;
    PackageSender& operator = (PackageSender&&) = default;
protected:
    void push_package(Package&& package);

public:
    ReceiverPreferences receiver_preferences_;
private:
    std::optional<Package> buffor_package_{std::nullopt};
};


class Ramp : public PackageSender{
public:
    Ramp(ElementID id, TimeOffset di);
    void deliver_goods(Time t) { if(t%di_==1 || di_==1) push_package(Package()); }
    TimeOffset get_delivery_interval() const { return di_; }
    ElementID get_id() const { return id_; }
    ~Ramp() override { if (id_!=0)  Ramp_IDs.erase(id_); }


    Ramp(const Ramp&) = delete;
    Ramp& operator = (const Ramp&) = delete;
    Ramp(Ramp&&) = default;
    Ramp& operator = (Ramp&&) = default;
private:
    static std::set<ElementID> Ramp_IDs;
    ElementID id_;
    TimeOffset di_;
};


class Worker : public PackageSender, public IPackageReceiver{
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q=std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    void do_work(Time t);
    TimeOffset get_processing_duration() const { return pd_; }
    Time get_package_processing_start_time() const { return start_t;}
    void receive_package (Package&& p) override { q_->push(std::move(p)); }// worker ma zawierac bufor do wysylki - ten od PS , swoja kolejke FIFO/LIFO i bufor aktualny
    ElementID get_id() const override { return id_; }
//    ReceiverType get_receiver_type() const override { return rt_; };
    IPackageStockpile::const_iterator cbegin() const override {return q_->cbegin();} //czy o to chodzilo z metodami delegujacymi?
    IPackageStockpile::const_iterator cend() const override {return q_->cend();}
    IPackageStockpile::const_iterator begin() const override {return q_->cbegin();}
    IPackageStockpile::const_iterator end() const override {return q_->cend();}
    ~Worker() override { if (id_!=0) Worker_IDs.erase(id_); }

    Worker(const Worker&) = delete;
    Worker& operator = (const Worker&) = delete;
    Worker(Worker&&) = default;
    Worker& operator = (Worker&&) = default;
private:
    static std::set<ElementID> Worker_IDs;
    ElementID id_;
    TimeOffset pd_; //pd - processing duration
    Time start_t;
    ReceiverType rt_;
    std::unique_ptr<IPackageQueue> q_;
    std::optional<Package> processing_buffer_{std::nullopt};
};


class Storehouse: public IPackageReceiver{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d=std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    void receive_package (Package&& p) override { d_->push(std::move(p)); }
    ElementID get_id() const override { return id_; }
//    ReceiverType get_receiver_type() const override { return rt_; };
    IPackageStockpile::const_iterator cbegin() const override {return d_->cbegin();} //o to chodzilo z metodami delegujacymi (wywoluja te [nadpisane] z klasy IPackageStockpile)
    IPackageStockpile::const_iterator cend() const override {return d_->cend();}
    IPackageStockpile::const_iterator begin() const override {return d_->cbegin();}
    IPackageStockpile::const_iterator end() const override {return d_->cend();}
    ~Storehouse() override { if (id_!=0) Storehouse_IDs.erase(id_); }

    Storehouse(const Storehouse&) = delete;
    Storehouse& operator = (const Storehouse&) = delete;
    Storehouse(Storehouse&&) = default;
    Storehouse& operator = (Storehouse&&) = default;
private:
    static std::set<ElementID> Storehouse_IDs;
    ElementID id_;
    ReceiverType rt_;
    std::unique_ptr<IPackageStockpile> d_;
};



#endif //NET_SIM_NODES_HPP
