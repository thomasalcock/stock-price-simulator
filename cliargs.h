#ifndef CLIARGS_H
#define CLIARGS_H

#include <string>

struct CommandLineArgs
{
    size_t n_paths;
    double mu;
    double sigma;
    double initial_stock_price_value;
    double delta_t;
    double total_time;
};
template <typename T>
T parse_flag(std::string flag, T min, T max);

CommandLineArgs handle_command_line_arguments(const int argc, char **argv);

#endif