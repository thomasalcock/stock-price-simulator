#include <vector>
#include <iostream>
#include "cliargs.h"
#include "simulation_utils.h"

// TODO: log program activity to file on disk
// TODO: rewrite program after introducing void functions

int main(int argc, char *argv[])
{
    CommandLineArgs args = handle_command_line_arguments(argc, argv);
    size_t n_iterations = determine_n_steps(args.total_time, args.delta_t);

    std::vector<vec_dbl> paths(args.n_paths);
    for (size_t i = 0; i < args.n_paths; ++i)
    {
        paths.reserve(n_iterations);
    }
    vec_dbl mean_path(n_iterations);

    int status = run_simulation(
        paths,
        mean_path,
        n_iterations,
        args.n_paths,
        args.process_type,
        args.mu,
        args.sigma,
        args.delta_t,
        args.initial_stock_price_value);

    if (status == 0)
    {

        print_stock_prices(paths);
        print_stock_price(mean_path);

        save_csv_file(args.output_file_name, paths, mean_path);
    }
    else
    {
        std::cerr << "Error simulating paths\n";
    }
}
