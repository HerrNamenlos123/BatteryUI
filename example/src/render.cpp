
#include "HomeUI.h"
#include "GLFW/glfw3.h"

std::unique_ptr<UI> ui; // Global instance

void setupUI() {

    ui = BatteryUI::Setup<UI>([&] {     // The first parameter is a lambda function callback which when called
        glfwPostEmptyEvent();                               // tell you to re-render the screen (for example when a loading bar is
    });                                                     // visible. Can be ignored if framerate is always high. Any additional
                                                            // parameters are forwarded to your UI class constructor. Be aware that
                                                            // this callback may be called from any thread

}

void updateUI() {   // This is called before ImGui::NewFrame() to prevent glitches when live reloading fonts

    static float fontSize = Fonts::fontSize;
    if (ImGui::IsKeyPressed(ImGuiKey_UpArrow, false)) {
        fontSize += 1.f;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_DownArrow, false) && Fonts::fontSize > 2) {   // Must stay above 0
        fontSize -= 1.f;
    }
    Fonts::setFontSize(fontSize);
}

void renderUI() {
    BatteryUI::NewFrame();
    ui->draw();
    BatteryUI::EndFrame();
}

void shutdownUI() {
    BatteryUI::Shutdown(ui);      // This must always be called before the ImGui context gets destroyed (think about errors/exceptions)
}
