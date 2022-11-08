#include <glob.h>
#include "imgui.h"
#include "imgui_impl_glfw.hpp"
#include "imgui_impl_opengl2.hpp"
#include "StackSummary.hpp"
#include <stdio.h>
#include <vector>
#include <functional>
#include <filesystem>
#include <chrono>
#include <sstream>
#include <iostream>
#include "StackElement.hpp"
#include <fstream>
#include <string>
#include <stdio.h>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GLFW/glfw3.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description) { fprintf(stderr, "Glfw Error %d: %s\n", error, description); }

std::vector<std::string> glob(const std::string& pattern) {
    using namespace std;

    // glob struct resides on the stack
    glob_t glob_result;
    memset(&glob_result, 0, sizeof(glob_result));
    vector<string> filenames;

    // do the glob operation
    int return_value = glob(pattern.c_str(), GLOB_TILDE, NULL, &glob_result);
    if (return_value != 0) {
        globfree(&glob_result);
        return filenames;
    }

    // collect all the filenames into a std::list<std::string>
    for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
        filenames.push_back(string(glob_result.gl_pathv[i]));
    }

    // cleanup
    globfree(&glob_result);

    // done
    return filenames;
}

std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

int main(int ac, char** av) {

    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "KapProfiler", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about
        // Dear ImGui!).

        std::vector<std::string> kprofFiles = glob("../../*.kprof");
        char checkChar;
        for (std::string& kprofFileName : kprofFiles) {
            std::ifstream file(kprofFileName);

            if (!file.is_open() || !file.good()) {
                continue;
            }
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string buff = buffer.str();

            if (!buff.ends_with("\\")) {
                continue;
            }
            file.close();

            std::vector<std::string> lines = split(buffer.str(), "\n");
            if (lines.empty()) {
                continue;
            }
            int stackSummaryCount;

            try {
                stackSummaryCount = std::stoi(lines[0]);
            } catch (...) {
                std::cout << "error '" << lines[1] << "'" << std::endl;
                continue;
            }

            if (stackSummaryCount < 0) {
                break;
            }

            std::vector<KapEngine::Profiler::StackSummary> summaries;

            for (int itr = 0; itr < stackSummaryCount; ++itr) {
                int lid = 1 + itr;
                if (lid > lines.size() - 1) {
                    continue;
                }
                std::string line = lines[lid];
                std::vector<std::string> elements = split(lines[lid], "|");

                if (elements.size() != 6) {
                    continue;
                }
                std::string raw = elements[0];
                KapEngine::Profiler::StackElement elem(raw);
                try {
                    long long _callCount = std::stoll(elements[1]);
                    long long _totalTime = std::stoll(elements[2]);
                    long long _start = std::stoll(elements[3]);
                    long long _time = std::stoll(elements[4]);
                    long long _totTimeAllCalls = std::stoll(elements[5]);
                    KapEngine::Profiler::StackSummary summary(raw, _callCount, _totalTime, _start, _time, _totTimeAllCalls);
                    summaries.push_back(summary);
                } catch (...) {}
            }

            std::sort(summaries.begin(), summaries.end(), std::greater<>());
            ImGui::Begin(("Thread #" + kprofFileName).c_str());

            std::filesystem::file_time_type ftime = std::filesystem::last_write_time(kprofFileName);
            const auto systemTime = std::chrono::file_clock::to_time_t(ftime);
            long long now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            long long diff = (now - systemTime) / (long long)1000000000;

            ImGui::Text("Last updated: %lld s", diff);
            if (ImGui::Button("Reset")) {
                remove(kprofFileName.c_str());
            }
            static ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
                                           ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable;

            // When using ScrollX or ScrollY we need to specify a size for our table container!
            // Otherwise by default the table will fit all available space, like a BeginChild() call.
            ImVec2 outer_size = ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 8);
            if (ImGui::BeginTable("thread_table", 7, flags, outer_size)) {
                ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
                ImGui::TableSetupColumn("Class", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Method", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Calls", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Call/s", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Mean time per call (ms)", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Total time (ms)", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Time %", ImGuiTableColumnFlags_None);
                ImGui::TableHeadersRow();

                // Demonstrate using clipper for large vertical lists
                for (auto& summary : summaries) {
                    KapEngine::Profiler::StackElement elem(summary.getRaw());
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%s", elem.getClass().c_str());
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%s", elem.getMethod().c_str());
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%lld", summary.getCallCount());
                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%.2f", summary.getCallsPerSecond());
                    ImGui::TableSetColumnIndex(4);
                    ImGui::Text("%.2f", summary.getMeanCallPerSecond());
                    ImGui::TableSetColumnIndex(5);
                    ImGui::Text("%lld", summary.getTotalTime());
                    ImGui::TableSetColumnIndex(6);
                    ImGui::Text("%.2f", summary.getPercentageTime());
                }
                ImGui::EndTable();
            }

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        // If you are using this code with non-legacy OpenGL header/contexts (which you should not, prefer using imgui_impl_opengl3.cpp!!),
        // you may need to backup/reset/restore other state, e.g. for current shader using the commented lines below.
        // GLint last_program;
        // glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
        // glUseProgram(0);
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        // glUseProgram(last_program);

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
