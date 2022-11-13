#include "GlobImpl.hpp"

using namespace KapEngine::Profiler;

GlobImpl::GlobImpl(std::string& pattern) { _pattern = pattern; }

GlobImpl::~GlobImpl() {

}

std::vector<std::string> GlobImpl::retrieveGlobResult() {
    std::vector<std::string> filenames;

    try {
        glob_t glob_result;
        memset(&glob_result, 0, sizeof(glob_result));

        int return_value = glob(_pattern.c_str(), GLOB_TILDE, NULL, &glob_result);
        if (return_value != 0) {
            globfree(&glob_result);
            return filenames;
        }

        for (size_t i = 0; i < glob_result.gl_pathc; ++i) {
            filenames.push_back(std::string(glob_result.gl_pathv[i]));
        }

        globfree(&glob_result);
    } catch (...) { return filenames; }
    return filenames;
}