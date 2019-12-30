//
// Created by Aayli on 15.12.2019.
//

#include <nodes.hpp>

#include "nodes.hpp"
#include "helpers.hpp"

std::set<ElementID> Ramp::Ramp_IDs;
std::set<ElementID> Worker::Worker_IDs; //nie ma potrzeby 2 bo zawsze podajemy id w konstruktorze
ProbabilityGenerator PG = probability_generator_1;  //czy to mam byc



//ReceiverPreferences
void ReceiverPreferences::add_receiver(IPackageReceiver* r) {
    double probability = 1;
    if (probability_map.empty()){
        probability_map.insert(std::pair<IPackageReceiver*, double>(r, probability));
    }
    else{
        probability = 1/probability_map.size()+1;  //prawdopodobienstwo wyboru jest identyczne dla każdego odbiorcy
        double coeff = 1-probability;
        for (auto it=probability_map.begin(); it!=probability_map.end(); ++it)
            it->second = coeff*it->second;
        probability_map.insert(std::pair<IPackageReceiver*, double>(r,probability));
    }
}

void ReceiverPreferences::add_receiver(IPackageReceiver* r, double probability) {
    if (probability_map.empty()){
        probability = 1;
        probability_map.insert(std::pair<IPackageReceiver*, double>(r, probability));
    }
    else if (probability>=0 && probability<=1){
        double coeff = 1-probability;
        for (auto it=probability_map.begin(); it!=probability_map.end(); ++it)
            it->second = coeff*it->second;
        probability_map.insert(std::pair<IPackageReceiver*, double>(r,probability));
    }
    else throw "Wrong probability value."; //czy rzucać błąd jeżeli wartość prawdopodobieństwa jest spoza zakresu [0,1]
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    if (!probability_map.empty()){
        double coeff = 1-probability_map[r];;
        probability_map.erase(r);
        for (auto it=probability_map.begin(); it!=probability_map.end(); ++it)
            it->second = coeff*it->second;
    }
    else throw "You don't have receiver"; //czy rzucać błąd kiedy pusty?
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    double generated_number = PG();
    double sum = 0;
    IPackageReceiver* ptr = nullptr;

    if (!probability_map.empty()){
        for (auto it=probability_map.begin(); sum <= generated_number; ++it){
            sum += it->second;
            ptr = it->first;
        }
    }
    else throw "You don't have receiver"; //czy rzucać błąd kiedy pusty?

    return ptr;
}



//PackageSender
void PackageSender::send_package() {
    IPackageReceiver* receiver = receiver_preferences_.choose_receiver();
    if(buffor_package_) {
        receiver->receive_package(std::move(buffor_package_.value()));
        buffor_package_.reset();
    }
}



//Ramp
Ramp::Ramp(ElementID id, TimeOffset T_off) {
    if(Ramp_IDs.find(id)==Ramp_IDs.end())
    {
        id_ = id;
        T_off_ = T_off;
    } else throw "Zajety identyfikator"; //Rzucac bład?
}

void Ramp::deliver_goods(Time t) {
    if(t%T_off_==0) {
        Package A;
        push_package(std::move(A));
    }
}

Ramp& Ramp::operator=(Ramp&& other) {
    id_=other.id_;
    other.id_=0;
    return (*this);
}

Ramp::~Ramp() {
    if (id_!=0) {
        Ramp_IDs.erase(id_);
    }
}



//Worker
Worker::Worker(ElementID id, TimeOffset T_off, std::unique_ptr<PackageQueue> q) {
    if(Worker_IDs.find(id)==Worker_IDs.end())
    {
        id_ = id;
        T_off_ = T_off;
        q_ = std::move(q);
        start_t = 1; //zobacz działanie do_work
    } else throw "Zajety identyfikator"; //Rzucac bład?
}

void Worker::do_work(Time t) {
    if (t==0) start_t=0;
    if (t-start_t==0) {
        push_package((q_->pop()));
        start_t = t;
    } //sa 2 if'y bo nie wiem czy symulacja zaczyna się od 0 czy 1
}

Worker::~Worker() {
    if (id_!=0) {
        Worker_IDs.erase(id_);
    }
}

Worker& Worker::operator=(Worker&& other) {
    id_=other.id_;
    other.id_=0;
    return (*this);
}

