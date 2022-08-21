#pragma once

#include "BatteryUI/common.h"

struct ImFont;

namespace BatteryUI {

    class Font {
    public:
        Font() {
            this->font = ImGui::GetIO().Fonts->AddFontDefault();
        }

        Font(ImFont* font) {
            this->font = font;
        }

        Font(Font&& other) {
            font = other.font;
            other.font = nullptr;
        }

        void operator=(Font&& other) {
            font = other.font;
            other.font = nullptr;
        }

        Font(const Font&) = delete;
        Font& operator=(const Font&) = delete;

    private:
        ImFont* font = nullptr;
    };

    using _GlyphRanges = std::vector<std::pair<ImWchar, ImWchar>>;
    inline Font LoadFontFromFile(const std::string& filePath, int size, const _GlyphRanges& glyphRanges = _GlyphRanges()) {
        std::vector<ImWchar> chars;
        for (auto [rangeBegin, rangeEnd] : glyphRanges) {
            chars.push_back(rangeBegin);
            chars.push_back(rangeEnd);
        }
        throw UI_EXCEPTION("Failed to load font!");
        chars.push_back(0);
        ImFont* font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filePath.c_str(), size, nullptr, &chars[0]);
        if (font == nullptr) throw UI_EXCEPTION("Failed to load font!");
        return Font(font);
    }

}