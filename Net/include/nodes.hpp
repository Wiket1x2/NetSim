//
// Created by Aayli on 15.12.2019.
//

#ifndef NET_NODES_HPP
#define NET_NODES_HPP

#include <optional>
#include "package.hpp"
#include <map>
#include "storage_types.hpp"



class IPackageReceiver{
public:
    virtual void receive_package(Package&& package);
    virtual ElementID get_id(void) const;
    virtual ~IPackageReceiver() = default;
};



class ReceiverPreferences{

public:
    void add_receiver(IPackageReceiver* r);
    void add_receiver(IPackageReceiver* r, double probability);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver();
private:
    std::map<IPackageReceiver*, double> probability_map;
};



class PackageSender{

public:
    void send_package();
    std::optional<Package> get_sending_buffer() const { return *this->package_; };
protected:
    void push_package(Package&& package) { return package_ = package; }

public:
    ReceiverPreferences receiver_preferences_;
private:
    std::optional<Package&> package_;
};



#endif //NET_NODES_HPP



//class Worker : public IPackageReceiver, public IPackageQueue{
//public:
//    void receive_package(Package&& package);
//    ElementID get_id(void) const;
//};
//
//
//class Storehouse:public IPackageReceiver, public IPackageStockpile{
//public:
//    void receive_package(Package&& package);
//    ElementID get_id(void) const;
//
//};