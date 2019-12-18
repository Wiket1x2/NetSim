//
// Created by Aayli on 15.12.2019.
//


#include "storage_types.hpp"


void PackageQueue::push(Package&& package) {
    switch(storage_type_) {
        case PackageQueueType::FIFO:
            plist.emplace_back(std::move(package));
            break;
        case PackageQueueType::LIFO:
            plist.emplace_front(std::move(package));
            break;

    }

}

Package PackageQueue::pop()  {
    Package pack= std::move(*plist.begin()); //pack jest zmienna pomocnicza przechowujaca pierwszy element listy
    plist.pop_front(); //pobieram z przodu (wlasciwe zarowno dla kolejki jak i stosu, pod warunkiem dodawania w odpowiedni sposob)
    return pack;
}





