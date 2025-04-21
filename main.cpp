#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include "cliargs.h"
#include "simulation_utils.h"

int main(int argc, char *argv[])
{

    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<> distr(0.0, 1.0);
    std::normal_distribution<> normal_distr(0.0, 1.0);

    CommandLineArgs args = handle_command_line_arguments(argc, argv);

    std::vector<vec_dbl> paths(args.n_paths);
    vec_dbl prices;

    for (size_t i; i < args.n_paths; ++i)
    {
        prices = stochastic_process(
            args.process_type,
            args.initial_stock_price_value,
            args.mu,
            args.sigma,
            args.total_time,
            args.delta_t,
            normal_distr, engine);

        paths[i] = prices;
    }
    print_stock_prices(paths);
    auto avg = average_path(paths);
    print_stock_price(avg);
}
