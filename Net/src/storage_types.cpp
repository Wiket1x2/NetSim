//
// Created by Aayli on 15.12.2019.
//


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
            product = pdeq_.front();
            pdeq_.pop_front();
            break;
        case PackageQueueType::LIFO:
            product = pdeq_.back();
            pdeq_.pop_back();
            break;
    }
    return product;
}




