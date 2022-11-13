#include "Viewer.hpp"

using namespace KapEngine::Profiler;

Viewer::Viewer(std::string &filePattern) {
    _filePattern = filePattern;
}

bool Viewer::initializeWindow() {
    // Window is already initialized
    if (_windowInitialized) {
        return false;
    }
    _windowInitialized = true;
    // Set GLFW error callback
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        return false;
    }
    // Create GLFW window
    _window = glfwCreateWindow(1280, 720, "KapProfiler", NULL, NULL);
    if (_window == NULL) {
        return false;
    }
    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);
}

void Viewer::initializeImGui() {
    // Check ImGui version
    IMGUI_CHECKVERSION();

    // Create ImGui context
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    ImGui::StyleColorsDark();

    // Initialize implementation
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL2_Init();
}

void Viewer::errorCallback(int error, const char *content) {
    fprintf(stderr, "Error with GLFW - ID %d: %s\n", error, content);
}


void Viewer::run() {

}