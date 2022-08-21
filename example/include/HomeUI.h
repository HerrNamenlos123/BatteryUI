#pragma once

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "BatteryUI/BatteryUI.h"    // This must be included after imgui.h!

                // This is a container, which is created by BatteryUI::Setup() and deleted
struct UI {     // by BatteryUI::Shutdown() to ensure destruction at the right time.

    int fontSize = 18;
    BatteryUI::Font font;

    BatteryUI::Window window;

    UI() {
        loadFonts();
    }

    void loadFonts() {
        font = BatteryUI::LoadFontFromFile("../resources/roboto.medium.ttf", fontSize);
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

};

extern std::unique_ptr<UI> ui;
