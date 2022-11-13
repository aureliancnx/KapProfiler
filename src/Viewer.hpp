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
            std::string _filePattern;

          public:
            Viewer(std::string& filePattern);
            ~Viewer();

            void initializeWindow();
            void run();
        };

    } // namespace Profiler
} // namespace KapEngine

#endif // PROFILER_GLOBIMPL_HPP
