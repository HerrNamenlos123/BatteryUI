
#include "HomeUI.h"
#include "GLFW/glfw3.h"

std::unique_ptr<UI> ui; // Global instance

void setupUI() {
    ui = BatteryUI::Setup<UI>([&] {     // The first parameter is a lambda function callback which when called
        glfwPostEmptyEvent();           // tell you to re-render the screen (for example when a loading bar is
    });                                 // visible. Can be ignored if framerate is always high. Any additional
                                        // parameters are forwarded to your UI class constructor
    
}

void updateUI() {   // This is called before ImGui::NewFrame() to prevent glitches when live reloading fonts
    
    if (ImGui::IsKeyPressed(ImGuiKey_UpArrow, false)) {
        ui->fontSize++;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_DownArrow, false) && ui->fontSize > 4) {   // Must stay above 0
        ui->fontSize--;
    }
    ui->updateFontSize();
    ui->updateHotreload();
}

void renderUI() {
    BatteryUI::NewFrame();

    ui->window([&] {
        ImGui::Text("Hello");

        ui->save();
        ui->load();

		

        ui->drop1.items.clear();
        ui->drop1.items.push_back("fdsakl");
        ui->drop1.items.push_back("f3fewfa");
        ui->drop1.items.push_back("3094zt083");
        ui->drop1.items.push_back("foiewmfi0w");
        
        ui->drop1();
        ui->drop2();
    });
	
    ui->drawStyleManagerWindow();
	
    ImGui::PushFont(ui->font);
    ImGui::ShowDemoWindow();
    ImGui::PopFont();

    BatteryUI::EndFrame();
}

void shutdownUI() {
    BatteryUI::Shutdown(ui);      // This must always be called before the ImGui context gets destroyed (think about errors/exceptions)
}
