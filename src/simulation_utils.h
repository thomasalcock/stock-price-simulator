#ifndef SIMUTILS_H
#define SIMUTILS_H

#include "types.h"
#include <random>

enum class StochasticProcessType
{
    brownian,
    wiener,
};

void save_csv_file(const std::string &output_file_name,
                   const std::vector<vec_dbl> &paths,
                   const vec_dbl &mean_path);

// Don't use auto because this will generate implicit template functions
// which can lead to nasty linker errors that are hard to resolve

int run_simulation(std::vector<vec_dbl> &paths,
                   vec_dbl &avg,
                   const size_t n_paths,
                   const StochasticProcessType process_type,
                   const double mu,
                   const double sigma,
                   const double delta_t,
                   const double total_time,
                   const double initial_stock_price_value);

vec_dbl
stochastic_process(
    const StochasticProcessType type,
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