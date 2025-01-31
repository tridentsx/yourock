#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <libsigrok/libsigrok.h>

sr_context *sr_ctx = nullptr;
sr_session *sr_session_ctx = nullptr;
sr_dev_inst *sr_dev = nullptr;

void SetupWindowHints() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // No window borders
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE); // Always on top
}

bool InitializeSigrok() {
    if (sr_init(&sr_ctx) != SR_OK) {
        std::cerr << "Failed to initialize Sigrok" << std::endl;
        return false;
    }
    if (sr_session_new(sr_ctx, &sr_session_ctx) != SR_OK) {
        std::cerr << "Failed to create Sigrok session" << std::endl;
        return false;
    }
    return true;
}

float ReadMultimeterValue() {
    if (!sr_dev) return 0.0f;
    // Placeholder for actual multimeter reading logic
    return 1.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.5f));
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    SetupWindowHints();
    GLFWwindow* window = glfwCreateWindow(400, 200, "ImGui Overlay", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();

    if (!InitializeSigrok()) {
        return -1;
    }

    float voltage = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0, 0, 0, 0); // Transparent background
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowBgAlpha(0.3f); // Semi-transparent
        ImGui::SetNextWindowSize(ImVec2(250, 100));
        ImGui::Begin("Multimeter Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        
        voltage = ReadMultimeterValue();
        ImGui::Text("Voltage: %.2f V", voltage);
        
        ImGui::End();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    sr_session_destroy(sr_session_ctx);
    sr_exit(sr_ctx);

    return 0;
}

