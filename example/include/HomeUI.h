#pragma once

#include "Headers.h"
#include "OpenSans.h"

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

static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

static inline bool is_base64(uint8_t c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::vector<uint8_t> DecodeBase64(const std::string& data) {
    size_t in_len = data.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    uint8_t char_array_4[4], char_array_3[3];
    std::vector<uint8_t> ret;

    while (in_len-- && (data[in_] != '=') && is_base64(data[in_])) {
        char_array_4[i++] = data[in_]; in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = (uint8_t)base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret.push_back(char_array_3[i]);
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = (uint8_t)base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
    }

    return ret;
}

class UI : public BatteryUI::RootUI {
public:
    Window mainWindow;
    int fontIndex = 0;

    // Here you must add all widget names, otherwise they will not be part of the stylesheet. (Except fonts)
    BATTERYUI_SERIALIZE(UI, mainWindow);

    UI() : BatteryUI::RootUI(STYLESHEET) {
        BatteryUI::AddFontFromFile("roboto-medium", "../resources/roboto.medium.ttf", 18);
        std::string data("\0", OpenSansFontData_compressed_size);
        memcpy(data.data(), OpenSansFontData_compressed_data, OpenSansFontData_compressed_size);
        BatteryUI::AddFontFromEmbeddedArray("open-sans", data, 18);

        std::ifstream ifs("font.txt");
        if (!ifs.is_open()) throw UI_EXCEPTION("Failed font");
        std::string content( (std::istreambuf_iterator<char>(ifs) ),
                             (std::istreambuf_iterator<char>()    ) );
        auto d = DecodeBase64(content);
        std::string s(d.begin(), d.end());
        BatteryUI::AddFontFromMemfile("magneto", s, 18);
    }

    void draw() {           // This is the main rendering function of your UI
        std::vector<std::string> fonts;
        for (auto& [key, _] : BatteryUI::fontMap) {
            fonts.emplace_back(key);
        }
        BatteryUI::ScopedFont font(fonts[fontIndex % fonts.size()].c_str());

        //BatteryUI::ScopedFont font("roboto-medium");    // Push font using RAII

        // BatteryUI::PushFont("roboto-medium");    // Or explicitly
        // BatteryUI::PopFont();

        mainWindow();       // Render the main window and all of its children
        drawStyleManagerWindow();

        ImGui::ShowDemoWindow();    // Default ImGui demo window
    }

    void applyJsonRootUI(const nlohmann::json& json) override {
        from_json(json, *this);
    }

    void getJsonRootUI(nlohmann::json& json) override {
        to_json(json, *this);
    }
	
    BATTERYUI_STYLESHEET_HOTRELOAD();
};
