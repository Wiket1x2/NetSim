// 5: Kasztelewicz (302858), Szczerba (302924), Gorecki (302847)

#ifndef NET_SIM_STORAGE_TYPES_HPP
#define NET_SIM_STORAGE_TYPES_HPP

#include "package.hpp"

#include <list>


enum class PackageQueueType {FIFO, LIFO};

class IPackageStockpile {
public:
    using const_iterator=std::list<Package>::const_iterator;
    virtual void push(Package&& package) = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;
    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual std::size_t size() const = 0;
    virtual bool empty() const = 0;
    virtual ~IPackageStockpile() = default;
};

class IPackageQueue : public IPackageStockpile {
public:
    virtual PackageQueueType get_queue_type() const = 0;
    virtual Package pop() = 0;
    virtual ~IPackageQueue() override = default;
};

class PackageQueue : public IPackageQueue {
public:
    PackageQueue(PackageQueueType storage_type) : storage_type_(storage_type) {}
    void push(Package&& package) override;
    const_iterator cbegin() const override { return plist.cbegin(); }
    const_iterator cend() const override { return plist.cend(); }
    const_iterator begin() const override { return plist.cbegin(); }
    const_iterator end() const override { return plist.cend(); }
    std::size_t size() const override { return plist.size(); }
    bool empty() const override { return plist.empty(); }
    PackageQueueType get_queue_type() const override { return storage_type_; }
    Package pop() override;
    ~PackageQueue() override = default;

private:
    PackageQueueType storage_type_;
    std::list<Package> plist;
};



#endif //NET_SIM_STORAGE_TYPES_HPP

// 5: Kasztelewicz (302858), Szczerba (302924), Gorecki (302847)