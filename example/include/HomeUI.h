#pragma once

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "BatteryUI/BatteryUI.h"    // This must be included after imgui.h!

#define STYLESHEET "../resources/stylesheet.json"
#define ROBOTO_FONT "../resources/roboto.medium.ttf"

struct UI : BatteryUI::RootUI {

    int fontSize = 18;
    BatteryUI::Font font;

    BatteryUI::Window window;
    BatteryUI::Button save;
    BatteryUI::Button load;

    UI() : BatteryUI::RootUI(STYLESHEET) {
        loadFonts();
        save.name = "Save";
        load.name = "Load";
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
        EXPORT_ITEM(defaults);  // This is needed by RootUI
		
        EXPORT_ITEM(window);
        EXPORT_ITEM(load);
        EXPORT_ITEM(save);
    }

};
