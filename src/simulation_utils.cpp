#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include "simulation_utils.h"

void save_csv_file(const std::string &output_file_name,
                   const std::vector<vec_dbl> &paths,
                   const vec_dbl &mean_path)
{
    // TODO: add timestamp to filename
    std::ofstream output_file(output_file_name);

    for (size_t path_index = 0; path_index < paths.size(); ++path_index)
    {
        output_file << "path_" << path_index + 1 << ";";
    }
    output_file << "average_path;\n";
    for (size_t price_index = 0; price_index < paths[0].size(); ++price_index)
    {
        for (size_t path_index = 0; path_index < paths.size(); ++path_index)
        {
            output_file << paths[path_index][price_index] << ";";
        }

        output_file << mean_path[price_index] << "\n";
    }

    output_file.close();
    std::cout << output_file_name << " closed\n";
}

int run_simulation(std::vector<vec_dbl> &paths,
                   vec_dbl &avg,
                   const size_t n_paths,
                   const StochasticProcessType process_type,
                   const double mu,
                   const double sigma,
                   const double delta_t,
                   const double total_time,
                   const double initial_stock_price_value)
{
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<> distr(0.0, 1.0);
    std::normal_distribution<> normal_distr(0.0, 1.0);

    vec_dbl prices;
    for (size_t i = 0; i < n_paths; ++i)
    {
        prices = stochastic_process(
            process_type,
            initial_stock_price_value,
            mu,
            sigma,
            total_time,
            delta_t,
            normal_distr, engine);
        paths[i] = prices;
    }

    avg = average_path(paths);
    return 0;
}

vec_dbl stochastic_process(
    const StochasticProcessType type,
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
    double Z = 0;
    switch (type)
    {
    case StochasticProcessType::brownian:
        stock_prices[0] = initial_value;
        for (size_t t = 1; t < N; ++t)
        {
            Z = dist(engine);
            stock_prices[t] = stock_prices[t - 1] * exp((mu - (sigma * sigma) * 0.5) * delta_t + sigma * Z * sqrt(delta_t));
        }
        break;

    case StochasticProcessType::wiener:
        stock_prices[0] = 0;
        for (size_t t = 1; t < N; ++t)
        {
            Z = dist(engine);
            stock_prices[t] = stock_prices[t - 1] * delta_t + Z * sqrt(delta_t);
        }
        break;

    default:
        stock_prices[0] = initial_value;
        std::cout << "Generating geometric brownian motion by default\n";
        for (size_t t = 1; t < N; ++t)
        {
            Z = dist(engine);
            stock_prices[t] = stock_prices[t - 1] * exp((mu - (sigma * sigma) * 0.5) * delta_t + sigma * Z * sqrt(delta_t));
        }
        break;
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