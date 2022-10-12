#pragma once

#include "Headers.h"

struct Fonts : BatteryUI::Fonts {   // This is your central place to load fonts (Make sure everything is static!)
    static constexpr auto ROBOTO_FONT = "../resources/roboto.medium.ttf";

    static inline float fontSize = 18.f;
    static inline BatteryUI::Font robotoMain;

    static void loadFonts() {
        robotoMain = BatteryUI::LoadFontFromFile(ROBOTO_FONT, fontSize);
    }

    static void setFontSize(float size) {      // This is optional if you want to dynamically change the font size
        if (size == fontSize)
            return;

        fontSize = size;

        BatteryUI::ClearFontAtlas();
        Fonts::loadFonts();
        BatteryUI::BuildFontAtlas();

        ImGui_ImplOpenGL3_CreateFontsTexture(); // <- You must call this manually for your backend,
        // as BatteryUI does not know anything about your
        // (maybe custom) ImGui backend
    }
};