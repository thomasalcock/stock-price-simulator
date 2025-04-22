#include <string>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <set>

#include "cliargs.h"
#include "simulation_utils.h"

template <typename T>
T parse_flag(std::string flag, std::string name, T min, T max)
{
    auto flag_pair = parse_string_flag(flag, name);
    std::string flag_value = flag_pair.second;
    T value;
    try
    {
        if constexpr (std::is_same_v<T, size_t>)
        {
            value = std::stoul(flag_value);
            check_bounds(flag_value, flag_pair.first, value, min, max);
        }
        else if constexpr (std::is_same_v<T, double>)
        {
            value = std::stod(flag_value);
            check_bounds(flag_value, flag_pair.first, value, min, max);
        }
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Invalid argument: " << flag_pair.first << " " << flag_pair.second << "\n"
                  << e.what() << "\n";
        std::exit(1);
    }
    return value;
}

std::pair<std::string, std::string> parse_string_flag(const std::string flag, std::string name)
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

    return std::make_pair(flag_name, flag_value);
}

std::pair<std::string, std::string> split_string(const std::string &input, char split_char)
{
    size_t delimiter_position = input.find(split_char);
    if (delimiter_position == std::string::npos)
    {
        std::cerr << "Error splitting " << input << " by character " << split_char << "\n";
        std::exit(1);
    }
    std::pair<std::string, std::string> values;
    values.first = input.substr(0, delimiter_position);
    values.second = input.substr(delimiter_position + 1, input.size());
    std::cout << "Split " << input << " into " << values.first << " and " << values.second << "\n";
    return values;
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

StochasticProcessType convert_string_to_sp_type(const std::string &input)
{
    if (input == "brownian")
    {
        return StochasticProcessType::brownian;
    }
    else if (input == "wiener")
    {
        return StochasticProcessType::wiener;
    }
    else
    {
        return StochasticProcessType::brownian;
    }
}

void check_file_ext(const std::string &input)
{
    std::set<std::string> accepted_file_extensions = {"csv", "txt"};

    std::pair<std::string, std::string> split_values = split_string(input, '.');
    if (accepted_file_extensions.find(split_values.second) == accepted_file_extensions.end())
    {
        std::cerr << "File extension must be csv or txt\n";
        std::exit(1);
    }
}

CommandLineArgs
handle_command_line_arguments(const int argc, char **argv)
{
    if (argc != 9)
    {
        std::cerr << "Please specify the flags...\n";
        std::exit(1);
    }

    std::string n_paths_flag = argv[1];
    std::string drift_flag = argv[2];
    std::string volatility_flag = argv[3];
    std::string init_price_flag = argv[4];
    std::string delta_t_flag = argv[5];
    std::string total_time_flag = argv[6];
    std::string process_type_flag = argv[7];
    std::string output_file_flag = argv[8];

    CommandLineArgs cli_args;

    std::string type_flag = parse_string_flag(process_type_flag, "process_type").second;
    cli_args.process_type = convert_string_to_sp_type(type_flag);

    std::string output_file_value = parse_string_flag(output_file_flag, "output_file_name").second;
    check_file_ext(output_file_value);

    cli_args.output_file_name = output_file_value;

    cli_args.n_paths = parse_flag<size_t>(n_paths_flag, "n_paths", 1, 1000);
    cli_args.mu = parse_flag<double>(drift_flag, "drift", 0.05, 0.1);
    cli_args.sigma = parse_flag<double>(volatility_flag, "volatility", 0.01, 0.08);
    cli_args.initial_stock_price_value = parse_flag<double>(init_price_flag, "initial_value", -100, 100);
    cli_args.delta_t = parse_flag<double>(delta_t_flag, "delta_t", 0.001, 0.1);
    cli_args.total_time = parse_flag<double>(total_time_flag, "total_time", 0.1, 2.0);

    return cli_args;
}