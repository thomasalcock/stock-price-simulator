# TODO: vscode tasks to create build tasks for recreating build directories
# and makefiles
cmake_minimum_required(VERSION 3.22.1)

project(stock-price-simulator)

file(GLOB SOURCES "src/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/release)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror -Wextra -Wpedantic -pedantic-errors -g -O0 -fsanitize=address -std=c++20")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG} -O2 -DNDEBUG -fsanitize=address -std=c++20")

add_executable(stock-price-simulator ${SOURCES})