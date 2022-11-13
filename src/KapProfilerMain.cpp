#include <functional>
#include <string>

#include "Viewer.hpp"

int main(int ac, char** av) {
    if (!av || ac != 2) {
        std::cerr << "Usage: ./KapProfiler <path_of_kprof_files>" << std::endl;
        return 1;
    }

    std::string pattern = std::string(av[1]) + "/*.kprof";
    KapEngine::Profiler::Viewer viewer(pattern);

    viewer.run();
    return 0;
}
