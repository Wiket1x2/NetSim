//
// Created by Aayli on 15.12.2019.
//

#ifndef NET_TYPES_HPP
#define NET_TYPES_HPP
#include <cstdlib>
#include <iostream>
#include <random>
#include <ctime>
#include <functional>
#include "helpers.hpp"


using ElementID = unsigned int;
std::function<double()> ProbabilityGenerator = probability_generator_1;


#endif //NET_TYPES_HPP


//std::function<double()> ProbabilityGenerator = []() {
//    std::random_device rd;
//    std::mt19937 rng(rd());
//    return std::generate_canonical<double, 10>(rng);
//};