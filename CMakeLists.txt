cmake_minimum_required(VERSION 3.22.1)

project(stock-price-simulator)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/release)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror -Wextra -Wpedantic -pedantic-errors -g -O0 -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG")

set(SOURCES_CLI src/main.cpp src/cliargs.cpp)
set(SOURCES_GUI src/gui.cpp)

add_library(stock-price-simulator-utils STATIC src/simulation_utils.cpp)
add_executable(stock-price-simulator-cli ${SOURCES_CLI})
target_link_libraries(stock-price-simulator-cli PUBLIC stock-price-simulator-utils)

message(STATUS "OpenGL Libraries: ${OPENGL_LIBRARIES}")
message(STATUS "GLFW Found: ${glfw3_FOUND}")

if (glfw3_FOUND AND OpenGL_FOUND)

    add_library(imgui STATIC
        dependencies/imgui/imgui.cpp
        dependencies/imgui/misc/cpp/imgui_stdlib.cpp
        dependencies/imgui/imgui_draw.cpp
        dependencies/imgui/imgui_demo.cpp
        dependencies/imgui/imgui_tables.cpp
        dependencies/imgui/imgui_widgets.cpp
        dependencies/imgui/backends/imgui_impl_glfw.cpp
        dependencies/imgui/backends/imgui_impl_opengl3.cpp
    )

    add_library(
        implot STATIC 
        dependencies/implot/implot.cpp
        dependencies/implot/implot_items.cpp
    )

    # Include directories for ImGui
    target_include_directories(imgui PUBLIC
        dependencies/imgui
        dependencies/imgui/backends
        dependencies/imgui/misc
        dependencies/imgui/misc/cpp
    )

    # Include directories for Implot, also include ImGui's directories
    target_include_directories(implot PUBLIC
        dependencies/implot
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui  # Add ImGui include path
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui/backends
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui/misc
        ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui/misc/cpp
    )

    add_executable(stock-price-simulator-gui ${SOURCES_GUI})

    # Link the ImGui library to the GUI executable
    target_link_libraries(stock-price-simulator-gui stock-price-simulator-utils glfw OpenGL X11 imgui implot)

else()
    message(FATAL_ERROR "GLFW or OpenGL not found!")
endif()
