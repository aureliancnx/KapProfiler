#ifndef PROFILER_GLOBIMPL_HPP
#define PROFILER_GLOBIMPL_HPP

#include <glob.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>

namespace KapEngine {

    namespace Profiler {

        class Viewer {

          private:
            // Add file pattern
            std::string _filePattern;
            // Checks if the window is initialized
            bool _windowInitialized;

            /**
             * Initialize profiler window
             */
            void initializeWindow();
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
            void errorCallback(int, const char *);
        };

    } // namespace Profiler
} // namespace KapEngine

#endif // PROFILER_GLOBIMPL_HPP
