
#include "HomeUI.h"

std::unique_ptr<UI> ui; // Global instance

void setupUI() {
    ui = BatteryUI::Setup<UI>();   // Any parameters here are passed to the UI constructor
}

void updateUI() {   // This is called before ImGui::NewFrame() to prevent glitches when live reloading fonts
    
    if (ImGui::IsKeyPressed(ImGuiKey_UpArrow, false)) {
        ui->fontSize++;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_DownArrow, false) && ui->fontSize > 4) {
        ui->fontSize--;
    }
    ui->updateFontSize();   // This must be called before ImGui::NewFrame()

}

void renderUI() {
    BatteryUI::NewFrame();

    ui->window.draw([&] {
        ImGui::Text("Hello");
    });
	
    ImGui::PushFont(ui->font);
    ImGui::ShowDemoWindow();
    ImGui::PopFont();

    BatteryUI::EndFrame();
}

void shutdownUI() {
    BatteryUI::Shutdown(ui);      // This must always be called before the ImGui context gets destroyed (think about errors/exceptions)
}
