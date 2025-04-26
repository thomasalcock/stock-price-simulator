#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <fstream>

#include "cliargs.h"
#include "simulation_utils.h"

// TODO: output simulation results to a csv file (name, current timestamp)
// TODO: log program activity to file on disk
// TODO: extract main simulation function

int main(int argc, char *argv[])
{
    CommandLineArgs args = handle_command_line_arguments(argc, argv);
    std::vector<vec_dbl> paths(args.n_paths);
    vec_dbl mean_path;

    run_simulation(paths, mean_path, args.n_paths, args.process_type,
                   args.mu, args.sigma, args.delta_t,
                   args.total_time, args.initial_stock_price_value);

    std::ofstream output_file(args.output_file_name);

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

    print_stock_prices(paths);

    output_file.close();
    print_stock_price(mean_path);
}
