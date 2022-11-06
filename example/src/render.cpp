
#include "HomeUI.h"
#include "GLFW/glfw3.h"

std::unique_ptr<UI> ui; // Global instance

void setupUI() {

    BatteryUI::Config config;           // Here you must define some of your backend functions (directly or using lambdas)
    config.callback_requestRedraw = [&] { glfwPostEmptyEvent(); };
    config.callback_rebuildFontAtlas = [&] { ImGui_ImplOpenGL3_CreateFontsTexture(); };

    ui = BatteryUI::Setup<UI>(config);

    BatteryUI::theme = BatteryUI::Theme_Modern();   // You can use a predefined theme or create your own
}

void updateUI() {   // This is called before ImGui::NewFrame() to prevent glitches when live reloading fonts

    static float scale = 1.f;
    if (ImGui::IsKeyPressed(ImGuiKey_UpArrow, false)) {
        scale *= 1.1f;
        BatteryUI::SetFontScale(scale);
    }
    if (ImGui::IsKeyPressed(ImGuiKey_DownArrow, false)) {
        scale /= 1.1f;
        BatteryUI::SetFontScale(scale);
    }

    if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow, false)) {
        //ui->fontIndex--;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_RightArrow, false)) {
        //ui->fontIndex++;
    }
}

void renderUI() {
    BatteryUI::NewFrame();
    ui->draw();
    BatteryUI::EndFrame();
}

void shutdownUI() {
    BatteryUI::Shutdown(ui);      // This must always be called before the ImGui context gets destroyed (think about errors/exceptions)
}
