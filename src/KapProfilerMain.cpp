#include <glob.h>
#include <stdio.h>
#include <vector>
#include <functional>
#include <iostream>
#include <string>

#include "Viewer.hpp"

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "Graphical/imgui_impl_glfw.hpp"
#include "Graphical/imgui_impl_opengl2.hpp"
#include "StackSummary.hpp"
#include "StackElement.hpp"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

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

void initializeFolder() {}

int main(int ac, char** av) {
    if (!av || ac != 2) {
        return 1;
    }

    std::string pattern = av[1];
    Viewer viewer(pattern);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    std::string pattern = std::string(av[1]) + "/*.kprof";

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        GlobImpl glob(pattern);
        std::vector<std::string> kprofFiles = glob.retrieveGlobResults();
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
            std::cout << stackSummaryCount << std::endl;

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
            if (ImGui::Button("Reset")) {
                remove(kprofFileName.c_str());
            }
            static ImGuiTableFlags flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV |
                                           ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable;

            ImVec2 outer_size = ImVec2(0.0f, ImGui::GetTextLineHeightWithSpacing() * 8);
            if (ImGui::BeginTable("thread_table", 7, flags, outer_size)) {
                ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableSetupColumn("Class", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Method", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Calls", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Call/s", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Mean time per call (ms)", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Total time (ms)", ImGuiTableColumnFlags_None);
                ImGui::TableSetupColumn("Time %", ImGuiTableColumnFlags_None);
                ImGui::TableHeadersRow();

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

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
