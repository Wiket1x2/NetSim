//
// Created by krzys on 10.12.2019.
//

#ifndef NET_SIM_PACKAGE_HPP
#define NET_SIM_PACKAGE_HPP
#include <iostream>
#include <set>

using ElementID = unsigned int;

class Package{

private:
    static std::set<ElementID> assigned_IDs;
    static std::set<ElementID> freed_IDs;
    ElementID id;

public:
    Package() {
        // Take first el. from freed_IDs <set>, if it exists else take last from assigned_IDs. When assigned_IDs is empty first el. = 1.
        // Wez pierwszy el. z freed_IDs <set>, jezeli istnieje w przeciwnym wypadku wez ostatni el. z assigned_IDs. Kiedy assigned_IDs jest pusty pierwszym el. = 1
        if (!freed_IDs.empty()) {
            id = *freed_IDs.begin();
            freed_IDs.erase(id);
        } else
            id = *assigned_IDs.end() + 1;
        assigned_IDs.insert(id);
    }

    Package(Package&&) = default;

    ElementID get_id() const {return id;}

    Package& operator= (Package&&) = default;

    ~Package(){
    // Move id form assigned_IDs to freed_IDs
    // Przerzuc element z assigned_IDs do freed_IDs
        assigned_IDs.erase(id);
        freed_IDs.insert(id);
    }
};

#endif //NET_SIM_PACKAGE_HPP
