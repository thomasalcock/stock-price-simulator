#ifndef CLIARGS_H
#define CLIARGS_H

#include <string>
#include "simulation_utils.h"

struct CommandLineArgs
{
    size_t n_paths;
    double mu;
    double sigma;
    double initial_stock_price_value;
    double delta_t;
    double total_time;
    StochasticProcessType process_type;
};
template <typename T>
T parse_flag(std::string flag, T min, T max);

std::pair<std::string, std::string> parse_string_flag(
    const std::string flag, std::string name);

template <typename T>
void check_bounds(const std::string &flag_value,
                  const std::string &flag_name,
                  T value, T min, T max);

StochasticProcessType convert_string_to_sp_type(const std::string &input);

CommandLineArgs handle_command_line_arguments(const int argc, char **argv);

#endif