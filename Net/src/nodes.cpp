//
// Created by Aayli on 15.12.2019.
//

#include "nodes.hpp"



void ReceiverPreferences::add_receiver(IPackageReceiver* r) {
    double probability = 1;
    if (probability_map.empty){
        probability_map.insert(std::pair<IPackageReceiver*, double>(r, probability));
    }
    else{
        probability = ProbabilityGenerator();
        double coeff = 1-probability;
        for (auto it=probability_map.begin(); it!=probability_map.end(); ++it)
            it->second = coeff*it->secound;
        probability_map.insert(std::pair<IPackageReceiver*, double>(r,probability));
    }
}

void ReceiverPreferences::add_receiver(IPackageReceiver* r, double probability) {
    if (probability_map.empty){
        probability = 1;
        probability_map.insert(std::pair<IPackageReceiver*, double>(r, probability));
    }
    else if (probability>=0 && probability<=1){
        double coeff = 1-probability;
        for (auto it=probability_map.begin(); it!=probability_map.end(); ++it)
            it->second = coeff*it->secound;
        probability_map.insert(std::pair<IPackageReceiver*, double>(r,probability));
    }
    else throw "Wrong probability value.";
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    double coeff = 1-probability_map[r];;
    probability_map.erase(r);
    for (auto it=probability_map.begin(); it!=probability_map.end(); ++it)
        it->second = coeff*it->secound;
}

IPackageReceiver* ReceiverPreferences::choose_receiver(void) {
    double generated_number = ProbabilityGenerator();
    double sum = 0;
    IPackageReceiver* ptr;
    for (auto it=probability_map.begin(); sum <= generated_number; ++it){
        sum += it->secound;
        ptr = it;
    }
    return ptr;
}



void PackageSender::send_package(void) {

}

std::optional<Package> PackageSender::get_sending_buffer(void) {
    return std::optional<Package>();
}

void PackageSender::push_package(Package&&) {


}



