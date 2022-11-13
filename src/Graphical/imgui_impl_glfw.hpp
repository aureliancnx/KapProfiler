#pragma once
#include "imgui.h" // IMGUI_IMPL_API

struct GLFWwindow;
struct GLFWmonitor;

IMGUI_IMPL_API bool ImGui_ImplGlfw_InitForOpenGL(GLFWwindow* window, bool install_callbacks);
IMGUI_IMPL_API bool ImGui_ImplGlfw_InitForVulkan(GLFWwindow* window, bool install_callbacks);
IMGUI_IMPL_API bool ImGui_ImplGlfw_InitForOther(GLFWwindow* window, bool install_callbacks);
IMGUI_IMPL_API void ImGui_ImplGlfw_Shutdown();
IMGUI_IMPL_API void ImGui_ImplGlfw_NewFrame();

IMGUI_IMPL_API void ImGui_ImplGlfw_InstallCallbacks(GLFWwindow* window);
IMGUI_IMPL_API void ImGui_ImplGlfw_RestoreCallbacks(GLFWwindow* window);

IMGUI_IMPL_API void ImGui_ImplGlfw_WindowFocusCallback(GLFWwindow* window, int focused);      // Since 1.84
IMGUI_IMPL_API void ImGui_ImplGlfw_CursorEnterCallback(GLFWwindow* window, int entered);      // Since 1.84
IMGUI_IMPL_API void ImGui_ImplGlfw_CursorPosCallback(GLFWwindow* window, double x, double y); // Since 1.87
IMGUI_IMPL_API void ImGui_ImplGlfw_MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
IMGUI_IMPL_API void ImGui_ImplGlfw_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
IMGUI_IMPL_API void ImGui_ImplGlfw_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
IMGUI_IMPL_API void ImGui_ImplGlfw_CharCallback(GLFWwindow* window, unsigned int c);
IMGUI_IMPL_API void ImGui_ImplGlfw_MonitorCallback(GLFWmonitor* monitor, int event);
