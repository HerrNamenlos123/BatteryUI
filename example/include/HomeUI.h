#pragma once

#include "Headers.h"
#include "Fonts.h"

#define STYLESHEET "../resources/stylesheet.json"

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

    Window() {
        window.name = "BatteryUI Style Manager";
    }

    void operator()() {
        window([&] {
            ImGui::Text("Hello");

            save();
            grid1();
            load();

            drop1.items.clear();
            drop1.items.emplace_back("fdsakl");
            drop1.items.emplace_back("f3fewfa");
            drop1.items.emplace_back("3094zt083");
            drop1.items.emplace_back("foiewmfi0w");

            drop1();
            drop2();
        });
    }

    BATTERYUI_SERIALIZE(Window, window, save, load, drop1, drop2, button, grid1, grid2);
};

class UI : public BatteryUI::RootUI {
public:
    Window mainWindow;

    // Here you must add all widget names, otherwise they will not be part of the stylesheet. (Except fonts)
    BATTERYUI_SERIALIZE(UI, mainWindow);

    UI() : BatteryUI::RootUI(STYLESHEET) {
        Fonts::loadFonts();
    }

    void draw() {           // This is the main rendering function of your UI
        Fonts::robotoMain.push();

        mainWindow();       // Render the main window and all of its children
        drawStyleManagerWindow();

        ImGui::ShowDemoWindow();    // Default ImGui demo window

        Fonts::robotoMain.pop();
    }

    void applyJsonRootUI(const nlohmann::json& json) override {
        from_json(json, *this);
    }

    nlohmann::json getJsonRootUI() override {
        return *this;
    }
};
