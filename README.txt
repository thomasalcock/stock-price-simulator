Build the project my installing cmake (minimum version 3.22.1) and then run:

./build.sh Debug
./build.sh Release

Usage:

./build/release/stock-price-simulator n_paths=3 \
                drift=0.06 \
                volatility=0.04 \
                initial_value=100 \
                delta_t=0.005 \
                total_time=1.5 \
                process_type=brownian \
                output_file_name=new_results.csv