#include <iostream>
#include <vector>
#include <cmath>
#include "simulation_utils.h"

vec_dbl stochastic_process(
    const double initial_value,
    const double mu,
    const double sigma,
    const double T,
    const double delta_t,
    std::normal_distribution<double> &dist,
    std::mt19937 &engine)
{
    size_t N = (size_t)T / delta_t;
    vec_dbl stock_prices(N, 0);
    stock_prices[0] = initial_value;
    double Z = 0;
    for (size_t t = 1; t < N; ++t)
    {
        Z = dist(engine);
        stock_prices[t] = stock_prices[t - 1] * exp((mu - (sigma * sigma) * 0.5) * delta_t + sigma * Z * sqrt(delta_t));
    }
    return stock_prices;
}

void print_stock_prices(const std::vector<vec_dbl> &values)
{
    for (size_t j = 0; j < values.size(); ++j)
    {
        const auto &path = values[j];
        std::cout << "Path " << j + 1 << " ";
        for (size_t i = 0; i < path.size(); ++i)
        {
            std::cout << path[i] << " ";
        }
        std::cout << "\n";
    }
}

void print_stock_price(const vec_dbl &values)
{
    for (size_t i = 0; i < values.size(); ++i)
    {
        std::cout << values[i] << " ";
    }
    std::cout << "\n";
}

vec_dbl average_path(const std::vector<vec_dbl> &values)
{
    size_t N = values[0].size();
    vec_dbl averages(N);
    size_t NPATHS = values.size();
    double sum = 0;
    for (size_t i = 0; i < N; ++i)
    {
        for (size_t j = 0; j < NPATHS; ++j)
        {
            sum += values[j][i];
        }
        averages[i] = sum / NPATHS;
        sum = 0;
    }
    return averages;
}