#pragma once

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"

#include "BatteryUI/BatteryUI.h"    // This must be included AFTER ImGui!

#define STYLESHEET "../resources/stylesheet.json"
#define ROBOTO_FONT "../resources/roboto.medium.ttf"

struct UI : BatteryUI::RootUI {

    int fontSize = 18;
    BatteryUI::Font font;

    BatteryUI::Window window;
    BatteryUI::Button save;
    BatteryUI::Button load;
    BatteryUI::Dropdown drop1;
    BatteryUI::Dropdown drop2;
    BatteryUI::HorizontalGrid grid1 = {
            new BatteryUI::VerticalGrid({
                { new BatteryUI::Button("Button1"), "80px"},
                { new BatteryUI::Button("Button2"), "1"},
                { new BatteryUI::Button("Button3"), "80px"}
            }),
            new BatteryUI::Button("Button4"),
        };

    UI() : BatteryUI::RootUI(STYLESHEET) {
        loadFonts();
        save.name = "Save";
        load.name = "Load";
        drop1.name = "Dropdown";
        drop2.name = "Hehe";
    }

    void loadFonts() {
        font = BatteryUI::LoadFontFromFile(ROBOTO_FONT, fontSize);
    }

    void updateFontSize() {
        static int oldSize = 0;

        if (fontSize != oldSize) {
            oldSize = fontSize;

            BatteryUI::ClearFontAtlas();
            loadFonts();
            BatteryUI::BuildFontAtlas();
            ImGui_ImplOpenGL3_CreateFontsTexture(); // <- You must call this manually for your backend, 
                                                    // as BatteryUI does not know anything about your 
                                                    // (maybe custom) ImGui backend
        }
    }

    EXPORT_STYLE() {
        EXPORT_ITEM(defaults);  // <- This is needed by RootUI, do not forget!
		
        EXPORT_ITEM(window);
        EXPORT_ITEM(load);
        EXPORT_ITEM(save);
    }

};
