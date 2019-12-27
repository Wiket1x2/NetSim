//
// Created by Aayli on 15.12.2019.
//

#ifndef NET_NODES_HPP
#define NET_NODES_HPP

#include <optional>
#include "package.hpp"
#include <map>
#include "storage_types.hpp"



class ReceiverPreferences{

public:
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver(void);

private:
    std::map<IPackageReceiver*, double> probability_map;
};


class PackageSender: public ReceiverPreferences{

public:
    void send_package(void);
    std::optional<Package> get_sending_buffer(void);

protected:
    void push_package(Package&&);

public:
    ReceiverPreferences receiver_preferences_;
};


#endif //NET_NODES_HPP


//class IPackageReceiver{
//public:
//    virtual void receive_package(Package&& package);
//    virtual ElementID get_id(void) const;
//    virtual ~IPackageReceiver() = default;
//};
//
//
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