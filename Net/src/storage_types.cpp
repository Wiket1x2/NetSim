//
// Created by Aayli on 15.12.2019.
//


//
// Created by krzys on 10.12.2019.
//

#ifndef NET_SIM_STORAGE_TYPES_HPP
#define NET_SIM_STORAGE_TYPES_HPP

#include "package.hpp"
#include <deque>

enum class PackageQueueType {
    FIFO,
    LIFO,
};

class IPackageStockpile {
public:

    using deqP_ci=std::deque<Package>::const_iterator;
    virtual void push(Package&& package) =0; // query - to oznacza ze metoda jest const? ale jak to? przeciez to jest pushparametr const czy nie?
    virtual deqP_ci cbegin() const =0;
    virtual deqP_ci cend() const=0;
    virtual deqP_ci begin() const=0;
    virtual deqP_ci end() const=0;
    virtual std::size_t size() const=0;
    virtual bool empty() const=0;
    virtual ~IPackageStockpile()= 0; //=default? wirtualny czy czysto wirtualny?


};

class

#endif //NET_SIM_STORAGE_TYPES_HPP
