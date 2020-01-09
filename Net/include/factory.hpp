//
// Created by krzys on 10.12.2019.
//

#ifndef NET_SIM_FACTORY_HPP
#define NET_SIM_FACTORY_HPP

//TODO: Podzieliłem zadania o podobnej funkcjonalności z moimi sugestiami kto co powinien wziąć.
//TODO: Pkt jest abstrakcją, która ma mówić, że dane zadanie jest bardziej czasochłonne - nie daje żadnej gwarancji ze tak jest ;)
//TODO: Zaznaczcie proszę przy nawiasie z waszym inisiałem "+" - mi pasuje, "-" - wolałbym nie.
class Factory{
    //TODO: 3pkt (K)
    void add_ramp(Ramp&&);
    void remove_ramp(id: ElementID);
    void remove_receiver(collection: NodeCollection<Node>&, id: ElementID);
    //TODO: 2pkt + Worker i Storehouse (T)
    NodeCollection<Ramp>::iterator find_ramp_by_id(id: ElementID);
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(id: ElementID) const;
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const;
    NodeCollection<Ramp>::const_iterator ramp_cend() const;
    //TODO: 6pkt (T)
    bool is_consistent() const;
    //TODO: 4pkt (R)
    void do_deliveries(Time);
    void do_package_passing();
    void do_work(Time);
};


//TODO: 4pkt (K)
class NodeCollection{
    void add(node: Node&);
    void remove_by_id(id_: ElementID);
    NodeCollection<Node>::iterator find_by_id(id_: ElementID);
    NodeCollection<Node>::const_iterator find_by_id(id_: ElementID) const
};
//TODO: testy 3pkt (R)

#endif //NET_SIM_FACTORY_HPP
