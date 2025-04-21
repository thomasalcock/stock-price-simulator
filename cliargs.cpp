#include <string>
#include <iostream>
#include "cliargs.h"

template <typename T>
T parse_flag(std::string flag, std::string name, T min, T max)
{
    size_t delimiter_position = flag.find("=");
    if (delimiter_position == std::string::npos)
    {
        std::cerr << "Flag names and values must be specified like this: name=value";
        std::exit(1);
    }
    std::string flag_name = flag.substr(0, delimiter_position);
    if (flag_name != name)
    {
        std::cerr << flag_name << " must match name " << name << "\n";
        std::exit(1);
    }
    std::string flag_value = flag.substr(delimiter_position + 1, flag.size());
    T value;
    try
    {
        if constexpr (std::is_same_v<T, size_t>)
        {
            value = std::stoul(flag_value);
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            value = std::stod(flag_value);
        }
        check_bounds(flag_value, flag_name, value, min, max);
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Invalid argument: " << flag_name << " " << flag_value << "\n"
                  << e.what() << "\n";
        std::exit(1);
    }
    return value;
}

template <typename T>
void check_bounds(const std::string &flag_value,
                  const std::string &flag_name,
                  T value, T min, T max)
{
    if (value < min || value > max)
    {
        std::cerr << "Value of " << flag_value
                  << " of flag " << flag_name
                  << " must be between "
                  << min << " and " << max << "\n";
        std::exit(1);
    }
}

CommandLineArgs handle_command_line_arguments(const int argc, char **argv)
{
    if (argc != 7)
    {
        std::cerr << "Please specify the flags...\n";
        std::exit(1);
    }

    std::string flag = argv[1];
    std::string flag2 = argv[2];
    std::string flag3 = argv[3];
    std::string flag4 = argv[4];
    std::string flag5 = argv[5];
    std::string flag6 = argv[6];

    CommandLineArgs cli_args;
    cli_args.n_paths = parse_flag<size_t>(flag, "n_paths", 1, 1000);
    cli_args.mu = parse_flag<double>(flag2, "drift", 0.05, 0.1);
    cli_args.sigma = parse_flag<double>(flag3, "volatility", 0.01, 0.08);
    cli_args.initial_stock_price_value = parse_flag<double>(flag4, "initial_value", -100, 100);
    cli_args.delta_t = parse_flag<double>(flag5, "delta_t", 0.001, 0.1);
    cli_args.total_time = parse_flag<double>(flag6, "total_time", 0.1, 2.0);
    // cli_args.stochastic_process_type = parse_flag<std::string>(flag7, "stochastic_process_type", "", "");
    return cli_args;
}