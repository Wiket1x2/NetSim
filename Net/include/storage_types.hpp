//
// Created by Aayli on 15.12.2019.
//

#ifndef NET_STORAGE_TYPES_HPP
#define NET_STORAGE_TYPES_HPP

#include "package.hpp"
#include <deque>
#include <list>

//uzupelnij consty

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

class IPackageQueue: public IPackageStockpile {
public:
    virtual PackageQueueType get_queue_type() const =0;
    virtual Package pop() =0;
};

class PackageQueue: public IPackageQueue {
public:
    PackageQueue(const PackageQueueType& storage_type): storage_type_(storage_type) {}
    void push(Package&& package) override;
    deqP_ci cbegin() const override { return pdeq_.cbegin(); }
    deqP_ci cend() const override {return pdeq_.cend(); }
    deqP_ci begin() const override { return pdeq_.cbegin();}
    deqP_ci end() const override {return pdeq_.cend();}
    std::size_t size() const override { return pdeq_.size(); }
    bool empty() const override {return pdeq_.empty();}
    PackageQueueType get_queue_type() const override { return storage_type_; }
    Package pop() override;


private:
    PackageQueueType storage_type_;
    std::deque<Package> pdeq_;
};




#endif //NET_STORAGE_TYPES_HPP
