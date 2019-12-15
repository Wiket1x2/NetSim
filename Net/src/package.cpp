#include "package.hpp"

std::set<ElementID> Package::assigned_IDs;
std::set<ElementID> Package::freed_IDs;

//Package::Package() {
//    // Take first el. from freed_IDs <set>, if it exists else take last from assigned_IDs. When assigned_IDs is empty first el. = 1.
//    // Wez pierwszy el. z freed_IDs <set>, jezeli istnieje w przeciwnym wypadku wez ostatni el. z assigned_IDs. Kiedy assigned_IDs jest pusty pierwszym el. = 1
//    if (!Package::freed_IDs.empty()){
//        id = *Package::freed_IDs.begin();
//        Package::freed_IDs.erase(id);
//    }
//    else
//        id = *Package::assigned_IDs.end()+1;
//    Package::assigned_IDs.insert(id);
//}
//
//Package::~Package(){
//    // Move id form assigned_IDs to freed_IDs
//    // Przerzuc element z assigned_IDs do freed_IDs
//    Package::assigned_IDs.erase(id);
//    Package::freed_IDs.insert(id);
//}