// 5: Kasztelewicz (302858), Szczerba (302924), Gorecki (302847)

#ifndef NET_SIM_FACTORY_HPP
#define NET_SIM_FACTORY_HPP

#include "nodes.hpp"
#include <stdexcept>
#include <iostream>


//TODO: Podzieliłem zadania o podobnej funkcjonalności z moimi sugestiami kto co powinien wziąć.
//TODO: Pkt jest abstrakcją, która ma mówić, że dane zadanie jest bardziej czasochłonne - nie daje żadnej gwarancji ze tak jest ;)
//TODO: Zaznaczcie proszę przy nawiasie z waszym inisiałem "+" - mi pasuje, "-" - wolałbym nie.

//TODO: 4pkt (R)+(K)
//FIXME UWAGA: Definicje metod klasy szablonowej umieszczamy tutaj
template <typename Node>
class NodeCollection{
public:
    //FIXME Co z konstruktorem? Domyslny?
    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add(Node& node) { list_of_nodes.push_back(std::move(node)); } //TODO problem - kontenery nie moga przechowywac referencji, nie bezposrednio

    NodeCollection<Node>::iterator find_by_id(ElementID id_) {
        NodeCollection<Node>::iterator it_of_found_node = std::find_if(list_of_nodes.begin(),list_of_nodes.end(),[id_](const Node& elem){ return elem.get_id()==id_;} );
        return it_of_found_node; //jak nie znajdzie to zwraca it tuz za koncem

//        for(iterator it = list_of_nodes.begin(); it != list_of_nodes.end(); ++it) {
//            if(it->get_id()==id_) return it; return iterator();}
    }
    NodeCollection<Node>::const_iterator find_by_id(ElementID id_) const {
        NodeCollection<Node>::const_iterator it_of_found_node = std::find_if(list_of_nodes.cbegin(),list_of_nodes.cend(),[id_](const Node& elem){ return elem.get_id()==id_;} );
        return it_of_found_node;

//        for(const_iterator it = list_of_nodes.begin(); it != list_of_nodes.end(); ++it) if(it->get_id()==id) return it; return iterator();
    }

    void remove_by_id(ElementID id_) {
        auto node_to_remove=find_by_id(id_);
        if (node_to_remove!=list_of_nodes.end()) {list_of_nodes.erase(node_to_remove);}
    }

    const_iterator cbegin() const { return list_of_nodes.cbegin(); }
    const_iterator cend() const { return list_of_nodes.cend(); }
    const_iterator begin() const { return list_of_nodes.cbegin(); }
    const_iterator end() const { return list_of_nodes.cend(); }
    iterator begin() { return list_of_nodes.begin(); }
    iterator end() { return list_of_nodes.end(); }

private:
    container_t list_of_nodes; //FIXME zmieniono nazwe - nie mozna uzywac nazw zarezerowanych dla kontenerow np. list vector itp
};


class Factory{
    //TODO: 3pkt (K)
//    void add_ramp(Ramp&&);
//    void remove_ramp(ElementID id);
//    void remove_receiver(NodeCollection<Node>& collection, ElementID id);
    //TODO: 2pkt (R) Done. Waiting for accept:
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) { return r_list.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const { return r_list.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const { return r_list.cbegin(); }
    NodeCollection<Ramp>::const_iterator ramp_cend() const { return r_list.cend(); }
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) { return w_list.find_by_id(id); }
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const { return w_list.find_by_id(id); }
    NodeCollection<Worker>::const_iterator worker_cbegin() const { return w_list.cbegin(); }
    NodeCollection<Worker>::const_iterator worker_cend() const { return w_list.cend(); }
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) { return s_list.find_by_id(id); }
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const { return s_list.find_by_id(id); }
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const { return s_list.cbegin(); }
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const { return s_list.cend(); }

    //TODO: 6pkt (T)
//    bool is_consistent() const;
    //TODO: 4pkt (R) Done in different way. Waiting for accept:
    void do_deliveries(Time t) { for(auto& idx : r_list) idx.deliver_goods(t); } //FIXME nie wiem jak urzyć tego: std::for_each(ramp_cbegin(), ramp_cend(), ???);
    void do_package_passing(); //FIXME to samo co wyzej
    void do_work(Time t) { for(auto& idx : w_list) idx.do_work(t); } //FIXME to samo co wyzej

private:
    NodeCollection<Ramp> r_list;
    NodeCollection<Worker> w_list;
    NodeCollection<Storehouse> s_list;
};

//TODO: testy 3pkt (R)

#endif //NET_SIM_FACTORY_HPP
// 5: Kasztelewicz (302858), Szczerba (302924), Gorecki (302847)