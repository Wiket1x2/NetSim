// 5: Kasztelewicz (302858), Szczerba (302924), Gorecki (302847)

#ifndef NET_SIM_FACTORY_HPP
#define NET_SIM_FACTORY_HPP

#include "nodes.hpp"
#include <stdexcept>
#include <iostream>



template <typename Node>
class NodeCollection{
public:
    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;
//TODO: Accepted!
    const_iterator cbegin() const { return list_of_nodes.cbegin(); }
    const_iterator cend() const { return list_of_nodes.cend(); }
    const_iterator begin() const { return list_of_nodes.cbegin(); }
    const_iterator end() const { return list_of_nodes.cend(); }
    iterator begin() { return list_of_nodes.begin(); }
    iterator end() { return list_of_nodes.end(); }
//TODO: Done. Waiting for accept:
    void add(Node& node) { if(find_by_id(node.get_id())==list_of_nodes.end()) list_of_nodes.push_back(std::move(node)); } //FIXME if gdy przedmiot jest już w kolekcji

    NodeCollection<Node>::iterator find_by_id(ElementID id_)
    { return std::find_if(list_of_nodes.begin(),list_of_nodes.end(),[id_](const Node& elem){return elem.get_id()==id_;}); }

    NodeCollection<Node>::const_iterator find_by_id(ElementID id_) const
    { return std::find_if(list_of_nodes.cbegin(),list_of_nodes.cend(),[id_](const Node& elem){return elem.get_id()==id_;}); }

    void remove_by_id(ElementID id_) {
        auto node_to_remove=find_by_id(id_);
        if (node_to_remove!=list_of_nodes.end()) {list_of_nodes.erase(node_to_remove);}
    }

private:
    container_t list_of_nodes;
};


class Factory{
//TODO: Accepted!
public:
    void add_ramp(Ramp&& R) {r_list.add(R);}
    void remove_ramp(ElementID id) { remove_receiver(r_list, id); }
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) { return r_list.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const { return r_list.find_by_id(id); }
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const { return r_list.cbegin(); }
    NodeCollection<Ramp>::const_iterator ramp_cend() const { return r_list.cend(); }

    void add_worker(Worker&& W) {w_list.add(W);}
    void remove_worker(ElementID id);
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) { return w_list.find_by_id(id); }
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const { return w_list.find_by_id(id); }
    NodeCollection<Worker>::const_iterator worker_cbegin() const { return w_list.cbegin(); }
    NodeCollection<Worker>::const_iterator worker_cend() const { return w_list.cend(); }

    void add_storehouse(Storehouse&& S) {s_list.add(S);}
    void remove_storehouse(ElementID id);
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) { return s_list.find_by_id(id); }
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const { return s_list.find_by_id(id); }
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const { return s_list.cbegin(); }
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const { return s_list.cend(); }

    //TODO: 6pkt (T)
    bool is_consistent() const {return true;};
//TODO: Done. Waiting for accept:
    void do_deliveries(Time t) { for(auto& idx : r_list) idx.deliver_goods(t); } //FIXME ew. std::for_each(ramp_cbegin(), ramp_cend(), [t](auto &buff){buff.deliver_goods(t);}) //chce zobaczyć czy to działa
    void do_package_passing();
    void do_work(Time t) { for(auto& idx : w_list) idx.do_work(t); }

private:
    template <class Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id) {collection.remove_by_id(id);}
    NodeCollection<Ramp> r_list;
    NodeCollection<Worker> w_list;
    NodeCollection<Storehouse> s_list;
};


#endif //NET_SIM_FACTORY_HPP
// 5: Kasztelewicz (302858), Szczerba (302924), Gorecki (302847)