//
// Created by krzys on 10.12.2019.
//

#ifndef NET_PACKAGE_HPP
#define NET_PACKAGE_HPP
#include <iostream>
#include <set>
#include "types.hpp"

class Package{

private:
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
    ElementID id_;

public:
    Package();
    Package(const Package&) = default;
    Package(Package&&) = default;
    ElementID get_id() const {return id_;}
    Package& operator= (Package&&) = default;
    ~Package();
};

#endif //NET_PACKAGE_HPP



