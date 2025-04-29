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
                   const size_t n_iterations,
                   const size_t n_paths,
                   const StochasticProcessType process_type,
                   const double mu,
                   const double sigma,
                   const double delta_t,
                   const double initial_stock_price_value)
{
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<> distr(0.0, 1.0);
    std::normal_distribution<> normal_distr(0.0, 1.0);

    vec_dbl prices(n_iterations);
    for (size_t i = 0; i < n_paths; ++i)
    {
        stochastic_process(
            prices,
            n_iterations,
            process_type,
            initial_stock_price_value,
            mu,
            sigma,
            delta_t,
            normal_distr, engine);

        paths[i] = prices;
    }

    average_path(avg, paths, n_paths, n_iterations);
    return 0;
}

size_t determine_n_steps(const double T, const double delta_t)
{
    size_t N = (size_t)T / delta_t;
    return N;
}

// TODO: this doesn't really work for a GUI because the user changes inputs within the same session
// which means vectors need to be resized. This function doesn't do that very well.
// I think it'l be much easier to allocate, size the vector upfront and then  pass it to the function
void stochastic_process(
    vec_dbl &stock_prices,
    const size_t N,
    const StochasticProcessType type,
    const double initial_value,
    const double mu,
    const double sigma,
    const double delta_t,
    std::normal_distribution<double> &dist,
    std::mt19937 &engine)
{
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

void average_path(
    vec_dbl &averages,
    const std::vector<vec_dbl> &values,
    const size_t NPATHS,
    const size_t N)
{
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
}