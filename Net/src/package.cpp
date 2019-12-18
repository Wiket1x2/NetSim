 #include "package.hpp"

 std::set<ElementID> Package::assigned_IDs;
 std::set<ElementID> Package::freed_IDs;

 Package::Package() {
     if (!freed_IDs.empty()) {
         id_= *freed_IDs.begin(); //przeciazone nie trzeba cbegin()
         freed_IDs.erase(id_);
         assigned_IDs.insert(id_);
     }
     else if (!assigned_IDs.empty()) {
         id_= *(assigned_IDs.rbegin())+1;
         assigned_IDs.insert(id_);
     }
     else {
         id_=1; //ID to liczba calkowita dodatnia
         assigned_IDs.insert(id_);
     }
 }


 Package::Package(Package&& other): id_(other.id_)  {
     other.id_=0;
 }


 Package& Package::operator=(Package&& other) {
     if (id_!=0) {
         assigned_IDs.erase(id_);
         freed_IDs.insert(id_);
     }
     id_=other.id_;
     other.id_=0; //0 reprezentuje id_ niedozwolone
     return (*this);
 }


 Package::~Package() {
     if (id_!=0) { //bo produkt ktory jest przypisywany - std::move(product) bedzie mial id_ ale nie wlasciwe....
         assigned_IDs.erase(id_);
         freed_IDs.insert(id_);
     }
 }