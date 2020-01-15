//
// Created by krzys on 10.12.2019.
//

#include <factory.hpp>

void Factory::remove_worker(ElementID id){

    auto ptr = find_worker_by_id(id);
    IPackageReceiver *buff = &(*ptr);
    remove_receiver(w_list, id);

    for(auto& elem : w_list){
        for(auto& elem_prefs : elem.receiver_preferences_){
            if(elem_prefs.first == buff){
                elem.receiver_preferences_.remove_receiver(elem_prefs.first);
                break;
            }
        }
    }

    for(auto& elem : r_list){
        for(auto& elem_prefs : elem.receiver_preferences_){
            if(elem_prefs.first == buff){
                elem.receiver_preferences_.remove_receiver(elem_prefs.first);
                break;
            }
        }
    }
}

void Factory::remove_storehouse(ElementID id){

    auto ptr = find_worker_by_id(id);
    IPackageReceiver *buff = &(*ptr);
    remove_receiver(s_list, id);

    for(auto& elem : w_list){
        for(auto& elem_prefs : elem.receiver_preferences_){
            if(elem_prefs.first == buff){
                elem.receiver_preferences_.remove_receiver(elem_prefs.first);
                break;
            }
        }
    }
}


void Factory::do_package_passing() {
    for(auto& idx : r_list) idx.send_package();
    for(auto& idx : w_list) idx.send_package();
}
