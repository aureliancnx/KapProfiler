#include "Viewer.hpp"

using namespace KapEngine::Profiler;

Viewer::Viewer(std::string &filePattern) {
    _filePattern = filePattern;
}

void Viewer::initializeWindow() {
    // Window is already initialized
    if (_windowInitialized) {
        return;
    }
    _windowInitialized = true;
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        return 1;
    }
    GLFWwindow *window = glfwCreateWindow(1280, 720, "KapProfiler", NULL, NULL);
    if (window == NULL) {
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL2_Init();
}

void Viewer::errorCallback(int error, const char *content) {
    fprintf(stderr, "Error with GLFW - ID %d: %s\n", error, content);
}


void Viewer::run() {

}