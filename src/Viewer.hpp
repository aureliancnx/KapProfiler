#ifndef PROFILER_VIEWER_HPP
#define PROFILER_VIEWER_HPP

#include <glob.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

// Graphical includes

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#include <GLFW/glfw3.h>
#include <imgui.h>
#include "Graphical/imgui_impl_opengl2.hpp"
#include "Graphical/imgui_impl_glfw.hpp"

#include "GlobImpl.hpp"
#include "Stack/StackElement.hpp"
#include "Stack/StackSummary.hpp"

namespace KapEngine {

    namespace Profiler {

        class Viewer {

          private:
            // Add file pattern
            std::string _filePattern;
            // Checks if the window is initialized
            bool _windowInitialized;
            // Viewer window
            GLFWwindow* _window;
            ImVec4 _clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

            /**
             * Initialize profiler window
             */
            bool initializeWindow();

            /**
             * Initialize ImGui
             */
            void initializeImGui();

            /**
             * Destroy everything
             */
            void destroy();

            /**
             * Draw
             */
            void draw();

            std::vector<std::string> split(std::string, std::string);

          public:
            Viewer(std::string& filePattern);
            ~Viewer();

            /**
             * You need to call run() in order to enable the profiler viewer
             */
            void run();

            /**
             * Error callback with GLFW3
             */
            static void errorCallback(int, const char*);
        };

    } // namespace Profiler
} // namespace KapEngine

#endif // PROFILER_VIEWER_HPP
