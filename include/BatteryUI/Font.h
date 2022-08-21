#pragma once

#include "BatteryUI/common.h"

struct ImFont;

namespace BatteryUI {

    class Font {
    public:
        Font() {}

        Font(ImFont* font) {
            this->font = font;
        }

        Font(Font&& other) noexcept {
            font = other.font;
            other.font = nullptr;
        }

        void operator=(Font&& other) noexcept {
            font = other.font;
            other.font = nullptr;
        }

        operator ImFont*() {
            if (!font) throw UI_EXCEPTION("Cannot access unloaded font!");
            return font;
        }

        Font(const Font&) = delete;
        Font& operator=(const Font&) = delete;

    private:
        ImFont* font = nullptr;
    };

    inline void ClearFontAtlas() {
		ImGui::GetIO().Fonts->Clear();
    }

	inline void BuildFontAtlas() {
		ImGui::GetIO().Fonts->Build();
	}

    using _GlyphRanges = std::vector<std::pair<ImWchar, ImWchar>>;
    inline Font LoadFontFromFile(const std::string& filePath, int size, const _GlyphRanges& glyphRanges = _GlyphRanges()) {
        std::vector<ImWchar> chars;
        for (auto [rangeBegin, rangeEnd] : glyphRanges) {
            chars.push_back(rangeBegin);
            chars.push_back(rangeEnd);
        }
        chars.push_back(0);
		
		ImWchar* glyphs = chars.size() > 1 ? chars.data() : nullptr;
        ImFont* font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filePath.c_str(), size, nullptr, glyphs);
        if (font == nullptr) throw UI_EXCEPTION("Failed to load font!");
        return Font(font);
    }

}
