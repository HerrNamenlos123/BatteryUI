
#include "imgui.h"
#include "BatteryUI/BatteryUI.h"    // This must be included after imgui.h!

                 // This is a container, which is created by BatteryUI::Setup() and deleted
struct _UI {     // by BatteryUI::Shutdown() to ensure destruction at the right time.

    BatteryUI::Font font;

    BatteryUI::Window window;
    
    _UI(int size) {
        //font = BatteryUI::LoadFontFromFile("C:\\Windows\\Fonts\\arial.ttf", size);
    }
};
std::unique_ptr<_UI> ui; // Global instance
//extern _UI ui;  // You can put this in a header if you need it in multiple files

void setupUI() {
    int fontSize = 15;
    ui = BatteryUI::Setup<_UI>(fontSize);   // Any parameters here appear in the _UI constructor
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
