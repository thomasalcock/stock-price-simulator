#ifndef SIMUTILS_H
#define SIMUTILS_H

#include "types.h"
#include <random>

// Don't use auto because this will generate implicit template functions
// which can lead to nasty linker errors that are hard to resolve

vec_dbl stochastic_process(
    const double initial_value,
    const double mu,
    const double sigma,
    const double T,
    const double delta_t,
    std::normal_distribution<double> &dist,
    std::mt19937 &engine);

void print_stock_prices(const std::vector<vec_dbl> &values);
void print_stock_price(const vec_dbl &values);
vec_dbl average_path(const std::vector<vec_dbl> &values);

#endif