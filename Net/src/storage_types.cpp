//
// Created by Aayli on 15.12.2019.
//


#ifndef NET_STORAGE_TYPES_HPP
#define NET_STORAGE_TYPES_HPP

#include "storage_types.hpp"


void PackageQueue::push(Package&& package) {
    switch(storage_type_) {
        case PackageQueueType::FIFO:
            pdeq_.emplace_back(std::move(package));
            break;
        case PackageQueueType::LIFO:
            pdeq_.emplace_front(std::move(package));
            break;

    }

}

Package PackageQueue::pop() {
    Package product;
    switch(storage_type_) {
        case PackageQueueType::FIFO:
            product=*pdeq_.begin();
            pdeq_.pop_front();
            break;
        case PackageQueueType::LIFO:
            product=*pdeq_.rend();
            pdeq_.pop_back();
            break;
    }
    return product;
}



#endif //NET_STORAGE_TYPES_HPP


