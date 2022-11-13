#ifndef PROFILER_GLOBIMPL_HPP
#define PROFILER_GLOBIMPL_HPP

#include <glob.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>

#include <GLFW/glfw3.h>

namespace KapEngine {

    namespace Profiler {

        class Viewer {

          private:
            // Add file pattern
            std::string _filePattern;
            // Checks if the window is initialized
            bool _windowInitialized;
            // Viewer window
            GLFWwindow *_window;

            /**
             * Initialize profiler window
             */
            bool initializeWindow();
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
            static void errorCallback(int, const char *);
        };

    } // namespace Profiler
} // namespace KapEngine

#endif // PROFILER_GLOBIMPL_HPP
