#include <iostream>
#include <cmath>
#include <random>
#include <vector>

// HEADERS
using vec_dbl = std::vector<double>;

vec_dbl simulate_wiener_process(const double T,
                                const double delta_t,
                                auto &dist,
                                auto &engine);

vec_dbl stochastic_process(
    const double initial_value,
    const double mu,
    const double sigma,
    const double T,
    const double delta_t,
    auto &dist,
    auto &engine);

void print_stock_prices(const std::vector<vec_dbl> &values);
void print_stock_price(const vec_dbl &values);
vec_dbl average_path(const std::vector<vec_dbl> &values);

template <typename T>
T parse_flag(std::string flag, T min, T max);

// IMPLEMENTATIONS
vec_dbl simulate_wiener_process(const double T,
                                const double delta_t,
                                auto &dist,
                                auto &engine)
{
    size_t N = (size_t)T / delta_t;
    std::printf("N = %ld, delta_t = %.2f, T = %.2f\n", N, delta_t, T);
    vec_dbl values(N, 0);
    double noise = 0;
    for (size_t t = 0; t < N; ++t)
    {
        noise = dist(engine);
        values[t + 1] = values[t] + noise * sqrt(delta_t);
    }
    return values;
}

vec_dbl stochastic_process(
    const double initial_value,
    const double mu,
    const double sigma,
    const double T,
    const double delta_t,
    auto &dist,
    auto &engine)
{
    size_t N = (size_t)T / delta_t;
    vec_dbl stock_prices(N, 0);
    stock_prices[0] = initial_value;
    double Z = 0;
    for (size_t t = 1; t < N; ++t)
    {
        Z = dist(engine);
        stock_prices[t] = stock_prices[t - 1] * exp((mu - (sigma * sigma) * 0.5) * delta_t + sigma * Z * sqrt(delta_t));
    }
    return stock_prices;
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

vec_dbl average_path(const std::vector<vec_dbl> &values)
{
    size_t N = values[0].size();
    vec_dbl averages(N);
    size_t NPATHS = values.size();
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
    return averages;
}

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

        if (value <= min || value >= max)
        {
            std::cerr << "Value of " << flag_value
                      << " of flag " << flag_name
                      << " must be between "
                      << min << " and " << max << "\n";
            std::exit(1);
        }
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "Invalid argument: " << flag_name << " " << flag_value << "\n"
                  << e.what() << "\n";
        std::exit(1);
    }
    return value;
}

int main(int argc, char *argv[])
{

    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<> distr(0.0, 1.0);
    std::normal_distribution<> normal_distr(0.0, 1.0);
    if (argc != 5)
    {
        std::cerr << "Please specify the flags...\n";
        std::exit(1);
    }

    std::string flag = argv[1];
    std::string flag2 = argv[2];
    std::string flag3 = argv[3];
    std::string flag4 = argv[4];

    auto n_paths = parse_flag<size_t>(flag, "n_paths", 1, 10);
    auto mu = parse_flag<double>(flag2, "drift", 0.05, 0.1);
    auto sigma = parse_flag<double>(flag3, "volatility", 0.01, 0.08);
    auto initial_stock_price_value = parse_flag<double>(flag4, "initial_value", -100, 100);

    double total_time = 1.0;
    double delta_t = 0.1;

    std::vector<vec_dbl>
        paths(n_paths);
    vec_dbl prices;
    for (size_t i; i < n_paths; ++i)
    {
        prices = stochastic_process(initial_stock_price_value, mu, sigma, total_time,
                                    delta_t, normal_distr, engine);
        paths[i] = prices;
    }
    print_stock_prices(paths);
    auto avg = average_path(paths);
    print_stock_price(avg);
}
