//
// Created by dkope on 15.12.2021.
//

#ifndef HELPERS_HPP_
#define HELPERS_HPP_


#include <random>
#include "types.hpp"

extern std::random_device rd;
extern std::mt19937 rng;

extern double default_probability_generator();

extern ProbabilityGenerator probability_generator;

#endif /* HELPERS_HPP_ */