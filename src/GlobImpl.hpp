#ifndef PROFILER_GLOBIMPL_HPP
#define PROFILER_GLOBIMPL_HPP

#include <glob.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>

namespace KapEngine {

    namespace Profiler {

        class GlobImpl {

          private:
            std::string _pattern;

          public:
            GlobImpl(std::string&);
            ~GlobImpl();

            std::vector<std::string> retrieveGlobResult();
        };

    } // namespace Profiler
} // namespace KapEngine

#endif // PROFILER_GLOBIMPL_HPP
