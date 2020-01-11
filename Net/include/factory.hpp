//
// Created by krzys on 10.12.2019.
//

#ifndef NET_SIM_FACTORY_HPP
#define NET_SIM_FACTORY_HPP

#include "nodes.hpp"



//TODO: Podzieliłem zadania o podobnej funkcjonalności z moimi sugestiami kto co powinien wziąć.
//TODO: Pkt jest abstrakcją, która ma mówić, że dane zadanie jest bardziej czasochłonne - nie daje żadnej gwarancji ze tak jest ;)
//TODO: Zaznaczcie proszę przy nawiasie z waszym inisiałem "+" - mi pasuje, "-" - wolałbym nie.

//TODO: 4pkt (R)+(K)
template <typename Node>
class NodeCollection{
public:
    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;
    void add(Node& node) { list.push_back(node); };
//    void remove_by_id(ElementID id);
    NodeCollection<Node>::iterator find_by_id(ElementID id) { for(iterator it = list.begin(); it != list.end(); ++it) if(it->get_id()==id) return it; return iterator(); } //FIXME: sprawdzić i nullptr(funkcja w sumie nic nie robi) czy throw? (main.cpp)
    NodeCollection<Node>::const_iterator find_by_id(ElementID id) const { for(const_iterator it = list.begin(); it != list.end(); ++it) if(it->get_id()==id) return it; return iterator(); } //FIXME: sprawdzić i nullptr czy throw?
    const_iterator cbegin() const { return list.cbegin(); }
    const_iterator cend() const { return list.cend(); }
    const_iterator begin() const { return list.cbegin(); }
    const_iterator end() const { return list.cend(); }
    iterator begin() { return list.begin(); }
    iterator end() { return list.end(); }

private:
    container_t list;
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
