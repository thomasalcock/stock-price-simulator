#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <fstream>

#include "cliargs.h"
#include "simulation_utils.h"

// TODO: output simulation results to a csv file (name, current timestamp)
// TODO: log program activity to file on disk

int main(int argc, char *argv[])
{

    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<> distr(0.0, 1.0);
    std::normal_distribution<> normal_distr(0.0, 1.0);

    CommandLineArgs args = handle_command_line_arguments(argc, argv);

    std::vector<vec_dbl> paths(args.n_paths);
    vec_dbl prices;

    std::ofstream output_file(args.output_file_name);

    for (size_t i = 0; i < args.n_paths; ++i)
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

    auto avg = average_path(paths);
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

        output_file << avg[price_index] << "\n";
    }

    print_stock_prices(paths);

    output_file.close();
    print_stock_price(avg);
}
