// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "implot.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>
#include "simulation_utils.h"

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
// #include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
// #include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// Main code
int main(int, char **)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    const char *glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    const char *glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    const char *title = "Stock price simulator";
    GLFWwindow *window = glfwCreateWindow(800, 600, title, nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    // IM_ASSERT(font != nullptr);

    // Our state
    // bool show_demo_window = true;
    // bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    static int n_paths = 5;
    static double mu = 0.05;
    // static float mu_f = static_cast<float>(mu);
    static double sigma = 0.02;
    // static float sigma_f = static_cast<float>(sigma);
    static double total_time = 1.0;
    static float total_time_f = static_cast<float>(total_time);
    static double delta_t = 0.01;
    static float delta_t_f = static_cast<float>(delta_t);
    static double initial_value = 100;
    static bool start_simulation = false;
    static bool save_file = false;
    static int finished = 1;
    static std::vector<vec_dbl> simulated_paths;
    static vec_dbl mean_path;
    static size_t clicks_on_sim_button = 0;
    static std::string file_name;
    static vec_dbl index;
    static bool n_paths_changed, delta_t_changed, total_time_changed;
    static size_t n_iterations;
    static vec_dbl returns;
    static int n_bins = 50;

    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y));
            ImGui::SetNextWindowPos(ImVec2(0, 0));

            ImGui::Begin(title, nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

            ImGui::Columns(2, "columns");
            ImGui::Text("Settings");

            // TODO: leak sanitizer encountered a fatal error after closing the application and previously saving a file
            // TODO: file picker window??
            ImGui::SetNextItemWidth(150.f);
            n_paths_changed = ImGui::SliderInt("Number of paths", &n_paths, 1, 10000);
            // n_paths_changed = ImGui::InputInt("Number of paths", &n_paths, 1, 100);
            ImGui::SetNextItemWidth(150.f);

            ImGui::InputDouble("Drift", &mu, 0.01, 0.01);
            ImGui::SetNextItemWidth(150.f);

            ImGui::InputDouble("Volatilty", &sigma, 0.01, 0.01);
            ImGui::SetNextItemWidth(150.f);

            total_time_changed = ImGui::SliderFloat("Total time", &total_time_f, 1.0, 2.0);
            total_time = static_cast<double>(total_time_f);
            ImGui::SetNextItemWidth(150.f);

            delta_t_changed = ImGui::SliderFloat("Delta time", &delta_t_f, 0.001, 0.1, "%.3f", 0);
            delta_t = static_cast<double>(delta_t_f);

            ImGui::InputInt("Number of bins", &n_bins);

            ImGui::InputText("file name", &file_name);
            start_simulation = ImGui::Button("Start simulation", ImVec2(150, 60));
            save_file = ImGui::Button("Save as .csv file", ImVec2(150, 60));

            // lesson here is to allocate memory based on user input BEFORE attempting to access data#
            // and draw it to the screen
            if (start_simulation || n_paths_changed || total_time_changed || delta_t_changed)
            {

                n_iterations = determine_n_steps(total_time, delta_t);

                simulated_paths.resize(n_paths);
                for (vec_dbl &path : simulated_paths)
                {
                    path.clear();
                    path.resize(n_iterations);
                }
                mean_path.clear();
                mean_path.resize(n_iterations);

                returns.clear();
                returns.resize(n_paths);

                index.resize(n_iterations);
                for (size_t i = 0; i < n_iterations; i++)
                {
                    index[i] = i;
                }
            }

            if (start_simulation)
            {
                clicks_on_sim_button++;
                std::cout << "Running simulation\n";

                finished = run_simulation(simulated_paths, mean_path, n_iterations, n_paths,
                                          StochasticProcessType::brownian, mu, sigma,
                                          delta_t, initial_value);

                for (int i = 0; i < n_paths; i++)
                {
                    returns[i] = log(simulated_paths[i].back() / simulated_paths[i][0]);
                }

                std::cout << "simulated " << n_paths << " paths\n";
                std::cout << finished << std::endl;
            }

            // TODO: explore why scrolling suffers when there are many paths
            ImGui::NextColumn();
            if (finished == 0)
            {
                if (ImPlot::BeginPlot("Simulated Paths", ImVec2(-1, 0), ImPlotFlags_NoLegend))
                {
                    for (size_t plot_index = 0; plot_index < (size_t)n_paths; ++plot_index)
                    {
                        ImPlot::PlotLine(
                            "", index.data(),
                            simulated_paths[plot_index].data(), index.size(),
                            ImPlotLineFlags_None);
                    }
                    ImPlot::PlotLine("Mean Path", index.data(), mean_path.data(), index.size());
                    ImPlot::EndPlot();
                }

                if (ImPlot::BeginPlot("Log Returns", ImVec2(-1, 0), ImPlotFlags_NoLegend))
                {
                    ImPlot::PlotHistogram("", returns.data(), returns.size(), n_bins);
                    ImPlot::EndPlot();
                }
            }

            if (save_file && finished == 0)
            {
                std::cout << "Saving file\n";
                save_csv_file(file_name, simulated_paths, mean_path);
            }

            ImGui::End();
        }
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
