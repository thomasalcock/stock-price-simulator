cmake_minimum_required(VERSION 3.22.1)

project(stock-price-simulator)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/release)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror -Wextra -Wpedantic -pedantic-errors -g -O0 -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG -static")

set(SOURCES_CLI src/main.cpp src/cliargs.cpp src/simulation_utils.cpp)
set(SOURCES_GUI src/gui.cpp src/simulation_utils.cpp)

add_executable(stock-price-simulator-cli ${SOURCES_CLI})

message(STATUS "OpenGL Libraries: ${OPENGL_LIBRARIES}")
message(STATUS "GLFW Found: ${glfw3_FOUND}")

if (glfw3_FOUND AND OpenGL_FOUND)

    set(SOURCES_IMGUI dependencies/imgui/imgui.cpp
                    dependencies/imgui/imgui_draw.cpp
                    dependencies/imgui/imgui_demo.cpp
                    dependencies/imgui/imgui_tables.cpp
                    dependencies/imgui/imgui_widgets.cpp
                    dependencies/imgui/backends/imgui_impl_glfw.cpp
                    dependencies/imgui/backends/imgui_impl_opengl3.cpp)

    include_directories(dependencies/imgui)
    include_directories(dependencies/imgui/backends)
    include_directories(dependencies/imgui/misc)

    add_executable(stock-price-simulator-gui ${SOURCES_GUI} ${SOURCES_IMGUI})

    target_link_libraries(stock-price-simulator-gui glfw OpenGL X11)

else()
    message(FATAL_ERROR "GLFW or OpenGL not found!")
endif()