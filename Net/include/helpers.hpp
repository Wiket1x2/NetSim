// 5: Kasztelewicz (302858), Szczerba (302924), Gorecki (302847)

#ifndef NET_SIM_HELPERS_HPP
#define NET_SIM_HELPERS_HPP


#include <functional>
#include <random>

#include "types.hpp"

extern std::random_device rd;
extern std::mt19937 rng;

extern double default_probability_generator();

extern ProbabilityGenerator probability_generator;


#endif //NET_SIM_HELPERS_HPP

// 5: Kasztelewicz (302858), Szczerba (302924), Gorecki (302847)