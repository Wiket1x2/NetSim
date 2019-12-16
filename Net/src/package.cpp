 #include "package.hpp"

 std::set<ElementID> Package::assigned_IDs;
 std::set<ElementID> Package::freed_IDs;

 Package::Package() {
     // Take first el. from freed_IDs <set>, if it exists else take last from assigned_IDs. When assigned_IDs is empty first el. = 1.
     // Wez pierwszy el. z freed_IDs <set>, jezeli istnieje w przeciwnym wypadku wez ostatni el. z assigned_IDs. Kiedy assigned_IDs jest pusty pierwszym el. = 1
     if (!freed_IDs.empty()) {
         id_= *freed_IDs.begin();
         freed_IDs.erase(id_);
         assigned_IDs.insert(id_);
     }
     else if (!assigned_IDs.empty()) {
         id_= *assigned_IDs.rbegin()+1;
         assigned_IDs.insert(id_);
     }
     else {
         id_=0;
         assigned_IDs.insert(id_);
     }
 }

 Package::~Package() {
     assigned_IDs.erase(id_);
     freed_IDs.insert(id_);
 }
