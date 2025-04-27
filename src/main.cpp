#include <vector>

#include "cliargs.h"
#include "simulation_utils.h"

// TODO: log program activity to file on disk

int main(int argc, char *argv[])
{
    CommandLineArgs args = handle_command_line_arguments(argc, argv);
    std::vector<vec_dbl> paths(args.n_paths);
    vec_dbl mean_path;

    run_simulation(paths, mean_path, args.n_paths, args.process_type,
                   args.mu, args.sigma, args.delta_t,
                   args.total_time, args.initial_stock_price_value);

    print_stock_prices(paths);
    print_stock_price(mean_path);
}
