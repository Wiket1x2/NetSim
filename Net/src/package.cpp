 #include "package.hpp"

 std::set<ElementID> Package::assigned_IDs;
 std::set<ElementID> Package::freed_IDs;

 Package::Package() {
     // Take first el. from freed_IDs <set>, if it exists else take last from assigned_IDs. When assigned_IDs is empty first el. = 1.
     // Wez pierwszy el. z freed_IDs <set>, jezeli istnieje w przeciwnym wypadku wez ostatni el. z assigned_IDs. Kiedy assigned_IDs jest pusty pierwszym el. = 1
     if (!freed_IDs.empty()) {
         id= *freed_IDs.begin();
         freed_IDs.erase(id);
         assigned_IDs.insert(id);
     }
     else if (!assigned_IDs.empty()) {
         id= *assigned_IDs.rbegin()+1;
         assigned_IDs.insert(id);
     }
     else {
         id=0;
         assigned_IDs.insert(id);
     }
 }

 Package::~Package() {
     assigned_IDs.erase(id);
     freed_IDs.insert(id);
 }
