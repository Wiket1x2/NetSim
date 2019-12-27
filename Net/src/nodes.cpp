//
// Created by Aayli on 15.12.2019.
//

#include "nodes.hpp"



void ReceiverPreferences::add_receiver(IPackageReceiver* r) {
    double probability = 1;
    if (probability_map.empty()){
        probability_map.insert(std::pair<IPackageReceiver*, double>(r, probability));
    }
    else{
        probability = ProbabilityGenerator();  //jakie ma byc prawdopodobienstwo
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
// skalowanie - Czy jeżeli mam 2 obiekty o prawdopodobieństwie równym 50% i dorzucę kolejny o wartości 50% to mam otrzymać 25/25/50 %, czy 33/33/33 %?
        double coeff = 1-probability;
        for (auto it=probability_map.begin(); it!=probability_map.end(); ++it)
            it->second = coeff*it->second;
        probability_map.insert(std::pair<IPackageReceiver*, double>(r,probability));
    }
    else throw "Wrong probability value."; //czy rzucać błąd jeżeli wartość prawdopodobieństwa jest spoza zakresu [0,1]
}//błedy numeryczne

void ReceiverPreferences::remove_receiver(IPackageReceiver* r) {
    double coeff = 1-probability_map[r];;
    probability_map.erase(r);
    for (auto it=probability_map.begin(); it!=probability_map.end(); ++it)
        it->second = coeff*it->second;
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    double generated_number = ProbabilityGenerator();
    double sum = 0;
    IPackageReceiver* ptr = nullptr;
    for (auto it=probability_map.begin(); sum <= generated_number; ++it){
        sum += it->second;
        ptr = it->first;
    }
    return ptr;
}



void PackageSender::send_package() {
    IPackageReceiver* receiver = receiver_preferences_.choose_receiver();
    if(get_sending_buffer()) {
        receiver->receive_package(*this->package_.value());
        *this->package_.reset()
    }
}




