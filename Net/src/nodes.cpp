//
// Created by krzys on 10.12.2019.
//

#include "nodes.hpp"
#include "helpers.hpp"

std::set<ElementID> Ramp::Ramp_IDs;
std::set<ElementID> Worker::Worker_IDs;
std::set<ElementID> Storehouse::Storehouse_IDs;


//ReceiverPreferences
void ReceiverPreferences::add_receiver(IPackageReceiver* r) {
    double probability = 1.0;
    if (probability_map.empty()){
        probability_map.insert(std::pair<IPackageReceiver*, double>(r, probability));
    }
    else{
        probability = 1.0/(probability_map.size()+1);
        double coeff = 1.0-probability;
        for (auto it=probability_map.begin(); it!=probability_map.end(); ++it)
            it->second = coeff*(it->second);
        probability_map.insert(std::pair<IPackageReceiver*, double>(r,probability));
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    if (!probability_map.empty()){
        if(probability_map.size()>1) {
            double coeff = 1.0 / probability_map[r] / (probability_map.size() - 1);
            probability_map.erase(r);
            for (auto it = probability_map.begin(); it != probability_map.end(); ++it)
                it->second = (coeff) * (it->second);
        }
        else if (probability_map.size()==1) probability_map.erase(r);
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    double generated_number = pg_(); //FIXME czy aby napewno ta liczba wygenerowana bedzie z przedialu 0-1?  (R) nie my podajemy funkcje jako argument (trudne do zabezpiecznenia)
    double sum = 0.0;
    IPackageReceiver* ptr = nullptr;

    if (!probability_map.empty()){
        for (auto it=probability_map.begin(); sum < generated_number; ++it){
            sum += it->second;
            ptr = it->first;
        }
    }
    else throw std::runtime_error("You don't have any receivers"); //FIXME czy rzucać błąd kiedy pusty? (R) mysle ze jak najbardziej - nie wiemy do kogo udał się produkt - produkt nam w najlepszym wypadku zniknie.
    return ptr;
}


//PackageSender
void PackageSender::send_package() {
    if(buffor_package_) {
        IPackageReceiver* receiver = receiver_preferences_.choose_receiver();
        receiver->receive_package(std::move(buffor_package_.value()));
        buffor_package_.reset();
    }
}

void PackageSender::push_package(Package&& package) {
    if(!buffor_package_.has_value()) { buffor_package_= std::move(package);}
    else throw std::runtime_error("buffor is already full!");
}


//Ramp
Ramp::Ramp(ElementID id, TimeOffset di) {
    if(Ramp_IDs.find(id)==Ramp_IDs.end())
    {
        id_ = id;
        di_ = di;
        Ramp_IDs.insert(id_);
    } else throw std::runtime_error("Zajety identyfikator");
}


//Worker
Worker::Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) {
    if(Worker_IDs.find(id)==Worker_IDs.end())
    {
        id_ = id;
        pd_ = pd;
        q_ = std::move(q);
        Worker_IDs.insert(id_);
        start_t = 1;
        rt_ = ReceiverType::WORKER;
    } else throw std::runtime_error("Zajety identyfikator");
}

void Worker::do_work(Time t) {
    if(!q_->empty() && !processing_buffer_.has_value()) {
        processing_buffer_.emplace(q_->pop());
        start_t=t;
    }
    if(processing_buffer_.has_value() && t-start_t==pd_-1) {
        push_package(std::move(processing_buffer_.value()));
        processing_buffer_.reset();
    }
}


//Storehouse
Storehouse::Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d) {
    if(Storehouse_IDs.find(id)==Storehouse_IDs.end())
    {
        id_ = id;
        d_=std::move(d);
        Storehouse_IDs.insert(id_);
        rt_ = ReceiverType::STOREHOUSE;
    } else throw std::runtime_error("Zajety identyfikator"); //FIXME Rzucac blad? (R) tak
}



// 5: Kasztelewicz (302858), Szczerba (302924), Gorecki (302847)