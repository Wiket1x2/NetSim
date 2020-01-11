//
// Created by krzys on 10.12.2019.
//

#include <factory.hpp>

void Factory::do_package_passing() {
    for(auto& idx : r_list) idx.send_package();
    for(auto& idx : w_list) idx.send_package();
}

