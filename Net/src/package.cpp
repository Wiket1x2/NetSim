// 5: Kasztelewicz (302858), Szczerba (302924), Gorecki (302847)

#include "package.hpp"

std::set<ElementID> Package::assigned_IDs;
std::set<ElementID> Package::freed_IDs;


void insert_if_not_exists(std::set<ElementID>& s, ElementID e) {
    if (s.find(e) == s.end()) {
        s.insert(e);
    }
}

void erase_if_exists(std::set<ElementID>& s, ElementID e) {
    if (s.find(e) != s.end()) {
        s.erase(e);
    }
}

Package::Package() {
    if (!freed_IDs.empty()) {
        id_ = *freed_IDs.begin(); //przeciazone nie trzeba cbegin()
        freed_IDs.erase(id_);
        assigned_IDs.insert(id_);
    } else if (!assigned_IDs.empty()) {
        id_ = *(assigned_IDs.rbegin()) + 1;
        assigned_IDs.insert(id_);
    } else {
        id_ = 1; //ID to liczba calkowita dodatnia
        assigned_IDs.insert(id_);
    }
}

Package::Package(ElementID id) {
    if (assigned_IDs.find(id) != assigned_IDs.end()) {
        throw std::invalid_argument("The ID of " + std::to_string(id) + " is already assigned!");
    }
    id_ = id;
    assigned_IDs.insert(id_);
    erase_if_exists(freed_IDs, id);
}

Package& Package::operator=(Package&& other) noexcept {
    if (id_ != 0) {
        assigned_IDs.erase(id_);
        freed_IDs.insert(id_);
    }
    id_ = other.id_;
    other.id_ = 0;
    return (*this);
}

Package::~Package() {
    if (id_ != 0) { //bo produkt ktory jest przypisywany - std::move(product) bedzie mial id_ ale nie wlasciwe....
        insert_if_not_exists(freed_IDs, id_);
        erase_if_exists(assigned_IDs, id_);
    }
}

// 5: Kasztelewicz (302858), Szczerba (302924), Gorecki (302847)