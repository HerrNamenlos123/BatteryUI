
// This is a barebones example with ImGui and GLFW

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <exception>
#include <BatteryUI/BatteryUI.h>

// These three functions build up the example in render.cpp
void setupUI();
void updateUI();
void renderUI();
void shutdownUI();

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main() {

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "BatteryUI example", NULL, NULL);
    if (window == NULL)
        return 1;

    glfwMakeContextCurrent(window);
    //glfwSwapInterval(1);          // Not enabling V-Sync: We have our own FPS limit

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);     // You must set up all of this yourself. BatteryUI needs an 
    ImGui_ImplOpenGL3_Init(glsl_version);           // already functioning ImGui project
    
    try {
        setupUI();
    }
    catch (const BatteryUI::Exception& e) {       // You should always catch exceptions
        fprintf(stderr, "BatteryUI::Exception was thrown: %s\n", e.what());
        glfwSetWindowShouldClose(window, true);
    }

    while (!glfwWindowShouldClose(window)) {
        //glfwPollEvents();     // <- This would let it run as fast as possible
        glfwWaitEvents();       // <- This waits until an event arrives, this is an easy way to only let your
		                        // application render when something changes. However, this is a problem
                                // with dynamic widgets like blinking cursors as they do not create events.
								// In this case you could use glfwPostEmptyEvent(); to force an event and
		                        // cause a screen update only when necessary. You could use another thread which
		                        // periodically causes a screen update, just fast enough so the cursor looks good.
                                // (Not in this example)
		                        //
		                        // Dynamic widgets like spinners or loading bars have their own background threads
								// and callbacks which then each cause a screen update when necessary.
		                        

        try {
            updateUI();
        }
        catch (const BatteryUI::Exception& e) {       // You should always catch exceptions
            fprintf(stderr, "BatteryUI::Exception was thrown: %s\n", e.what());
            glfwSetWindowShouldClose(window, true);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        try {
            renderUI();     // This function is defined in render.cpp, it does all the work
        }
        catch (const BatteryUI::Exception& e) {       // You should always catch exceptions
            fprintf(stderr, "BatteryUI::Exception was thrown: %s\n", e.what());
            glfwSetWindowShouldClose(window, true);
        }

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    try {
        shutdownUI();
    }
    catch (const BatteryUI::Exception& e) {       // You should always catch exceptions
        fprintf(stderr, "BatteryUI::Exception was thrown: %s\n", e.what());
        glfwSetWindowShouldClose(window, true);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
