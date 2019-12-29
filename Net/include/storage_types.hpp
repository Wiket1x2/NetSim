//
// Created by krzys on 15.12.2019.
//

#ifndef NET_STORAGE_TYPES_HPP
#define NET_STORAGE_TYPES_HPP

#include "package.hpp"
#include <deque>
#include <list>


enum class PackageQueueType {FIFO, LIFO};

class IPackageStockpile {
public:
    using deqP_ci=std::list<Package>::const_iterator;
    virtual void push(Package&& package) =0;
    virtual deqP_ci cbegin() const =0;
    virtual deqP_ci cend() const=0;
    virtual deqP_ci begin() const=0;
    virtual deqP_ci end() const=0;
    virtual std::size_t size() const=0;
    virtual bool empty() const=0;
    virtual ~IPackageStockpile()= default; //czy w klasach potomnych tez def konstuktory override? - tak
};

class IPackageQueue: public IPackageStockpile {
public:
    virtual PackageQueueType get_queue_type() const =0; //jaki typ zwracany - wartosc bo enumeration jest traktoway jak typ prosty
    virtual Package pop()  =0;
    virtual ~IPackageQueue() override= default;
};

class PackageQueue: public IPackageQueue {
public:
    PackageQueue(PackageQueueType storage_type): storage_type_(storage_type) {}
    void push(Package&& package) override;
    deqP_ci cbegin() const override { return plist.cbegin(); }
    deqP_ci cend() const override {return plist.cend(); }
    deqP_ci begin() const override { return plist.cbegin();}
    deqP_ci end() const override {return plist.cend();}
    std::size_t size() const override { return plist.size(); }
    bool empty() const override {return plist.empty();}
    PackageQueueType get_queue_type() const override { return storage_type_; }
    Package pop() override;
    ~PackageQueue() override= default;


private:
    PackageQueueType storage_type_;
    std::list<Package> plist;
};


#endif //NET_STORAGE_TYPES_HPP
