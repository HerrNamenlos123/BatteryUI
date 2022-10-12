#pragma once

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"

#include "BatteryUI/BatteryUI.h"    // This must be included AFTER ImGui!

#define STYLESHEET "../resources/stylesheet.json"
#define ROBOTO_FONT "../resources/roboto.medium.ttf"

class Window {
public:
    BatteryUI::Window window;

    BatteryUI::Button save;
    BatteryUI::Button load;
    BatteryUI::Dropdown drop1;
    BatteryUI::Dropdown drop2;

    BatteryUI::Button button;

    BatteryUI::VerticalGrid grid2 = {
            { [&] { button(); button(); }, "1"},
            { [&] { button(); button(); }, "1"},
    };
    BatteryUI::HorizontalGrid grid1 = {
            { [&] { grid2(); }, "1"},
            { [&] { grid2(); }, "1"},
    };

    Window() {}

    void operator()() {
        window([&] {
            ImGui::Text("Hello");

            save();
            grid1();
            load();

            //if (save.clicked) {
            //    saveStyleSheet();
            //}

            drop1.items.clear();
            drop1.items.emplace_back("fdsakl");
            drop1.items.emplace_back("f3fewfa");
            drop1.items.emplace_back("3094zt083");
            drop1.items.emplace_back("foiewmfi0w");

            drop1();
            drop2();
        });
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Window, window, save, load, drop1, drop2, button, grid1, grid2);
};

class UI : public BatteryUI::RootUI {
public:

    int fontSize = 18;
    BatteryUI::Font font;

    Window mainWindow;

    // Here you must add all widget names, otherwise they will not be part of the stylesheet. (Except fonts)
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(UI, mainWindow);

    UI() : BatteryUI::RootUI(STYLESHEET) {
        loadFonts();
    }

    void draw() {           // This is the main rendering function of your UI
        mainWindow();   // Render the window and all of its children
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

    void applyJsonRootUI(const nlohmann::json& json) override {
        from_json(json, *this);
    }

    nlohmann::json getJsonRootUI() override {
        return *this;
    }
};
