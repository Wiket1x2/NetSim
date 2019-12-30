//
// Created by Aayli on 15.12.2019.
//

#ifndef NET_NODES_HPP
#define NET_NODES_HPP

#include <optional>
#include "package.hpp"
#include <map>
#include "storage_types.hpp"


enum class NodesType{RAMP, WORKER, STOREHOUSE};

//To Do: Iteratory
class IPackageReceiver{
public:
    virtual void receive_package(Package&& package) =0;
    virtual ElementID get_id(void) const =0;
    virtual ~IPackageReceiver() = default;
}; //get_ID() z konspektu mi wynika ze powinno byc krotka.


//To Do: Iteratory
class ReceiverPreferences{
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    ReceiverPreferences() = default;
//    ReceiverPreferences(ProbabilityGenerator* PG): PG_(PG) {}  //Wyczytane z konspektu. Jak to ma działać?
    void add_receiver(IPackageReceiver* r); //kiedy const r?
    void add_receiver(IPackageReceiver* r, double probability); //dodałem funkcje z 2 argumentem
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver();
private:
    preferences_t probability_map;
//    ProbabilityGenerator PG_;    //Wyczytane z konspektu. Czy to ma być? Czy uchwyt do funkcji moze nie być wskaźnikiem?
};


// To Do: get_sending_buffer()
class PackageSender{
public:
    void send_package();
//    std::optional<Package> get_sending_buffer() const { return buffor_package_;} //Problem
protected:
    void push_package(Package&& package) { buffor_package_ = std::move(package); }

public:
    ReceiverPreferences receiver_preferences_;
private:
    std::optional<Package> buffor_package_;
};



class Ramp : public PackageSender{
public:
    Ramp(ElementID id, TimeOffset T_off);
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() const { return T_off_; }
    ElementID get_id() const { return id_; }

//Nie widzialem polcenia ale myśle ze nie zaszkodzi
    Ramp(const Package&)= delete;
    Ramp(Ramp&& other): id_(other.id_)  { other.id_=0; }
    Ramp& operator=(const Ramp&)= delete;
    Ramp& operator=(Ramp&& other);
    ~Ramp();

private:
    static std::set<ElementID> Ramp_IDs;
    ElementID id_;
    TimeOffset T_off_;
};



class Worker : public PackageSender, public IPackageReceiver{
public:
    Worker(ElementID id, TimeOffset T_off, std::unique_ptr<PackageQueue> q);
    void do_work(Time t);  //Mozliwe latwiejsze rozwiazanie
    TimeOffset get_processing_duration() const { return T_off_; }
    Time get_package_processing_start_time() const { return start_t;}
    void receive_package (Package&& package) override { q_->push(std::move(package)); }
    ElementID get_id(void) const override { return id_; }
    ~Worker() override;

//Nie widzialem polcenia ale myśle ze nie zaszkodzi
    Worker(const Package&)= delete;
    Worker(Worker&& other): id_(other.id_)  { other.id_=0; }
    Worker& operator=(const Worker&)= delete;
    Worker& operator=(Worker&& other);

private:
    static std::set<ElementID> Worker_IDs;
    ElementID id_;
    TimeOffset T_off_;
    Time start_t;
    std::unique_ptr<PackageQueue> q_;
};

//Magazyn półproduktów
//sprawdzić const


#endif //NET_NODES_HPP