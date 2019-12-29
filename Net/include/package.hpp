//
// Created by krzys on 10.12.2019.
//

#ifndef NET_PACKAGE_HPP
#define NET_PACKAGE_HPP
#include <iostream>
#include <set>
#include "types.hpp"
#include <memory>

class Package{

public:
    Package(); //czy mamy zamiescic drugi konstruktor Package(ElementID id): id_(id){} ktory bedzie tworzyl obiekt na podst przekazanego id - patrz wczytywanie z pliku?
    // dodac obsluge gdy id jest zajete - tak na jedno i drugie pytanie // podczas robienia tego if(id!=other.id)
    Package(const Package&)= delete;
    Package(Package&& other): id_(other.id_)  { other.id_=0; }
    ElementID get_id() const { return id_;}
    Package& operator=(const Package&)= delete;
    Package& operator=(Package&& other);
    ~Package();

private:
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
    ElementID id_;
};




#endif //NET_PACKAGE_HPP