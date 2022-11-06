#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/InternalDecl.h"

struct ImFont;

namespace BatteryUI {

    class Font {
    public:

        enum class FontType {
            NONE,
            RAW,
            FILE,
            DIRECT_CFG,
            MEMORY_RAW,
            MEMORY_COMPRESSED
        };

        Font() = default;

        explicit Font(ImFont* font) {
            this->font = font;
            type = FontType::RAW;
        }

        Font(const std::string& filepath,
             float size,
             std::optional<ImFontConfig> fontConfig = std::nullopt,
             const std::vector<std::pair<ImWchar, ImWchar>>& glyphRanges = {})
        {
            type = FontType::FILE;
            fontdata_filepath = filepath;
            fontdata_size = size;
            fontdata_config = fontConfig;
            applyGlyphs(glyphRanges);
            loadFont();
        }

        Font(const ImFontConfig& fontConfig) {
            type = FontType::DIRECT_CFG;
            fontdata_config = fontConfig;
            loadFont();
        }

        Font(FontType typeOfData, const std::string& data,
             float size,
             std::optional<ImFontConfig> fontConfig = std::nullopt,
             const std::vector<std::pair<ImWchar, ImWchar>>& glyphRanges = {})
        {
            if (typeOfData != FontType::MEMORY_RAW &&
                typeOfData != FontType::MEMORY_COMPRESSED) {
                throw UI_EXCEPTION("Failed to load font: The BatteryUI::Font Memory-Constructor was called with a non-memory FontType");
            }
            type = typeOfData;
            fontdata_memdata = data;
            fontdata_size = size;
            fontdata_config = fontConfig;
            applyGlyphs(glyphRanges);
            loadFont();
        }

        operator ImFont*() {
            if (!font) throw UI_EXCEPTION("Cannot access font: Font is not loaded");
            return font;
        }

        void push() {
            if (font)
                ImGui::PushFont(font);
        }

        void pop() {
            if (font)
                ImGui::PopFont();
        }

        void SetScale(float scale) {
            loadFont(scale);
        }

    private:
        void loadFont(float scale = 1.f) {
            auto& fonts = ImGui::GetIO().Fonts;

            switch (type) {
                case FontType::FILE:
                    if (!std::filesystem::exists(fontdata_filepath))
                        throw UI_EXCEPTION("Failed to load font from '%s': No such file or directory", fontdata_filepath.c_str());

                    font = fonts->AddFontFromFileTTF(fontdata_filepath.c_str(),
                                                     fontdata_size * scale,
                                                     fontdata_config.has_value() ? (&fontdata_config.value()) : nullptr,
                                                     fontdata_glyphs.size() > 1 ? fontdata_glyphs.data() : nullptr);
                    if (!font) throw UI_EXCEPTION("File at '%s' could not be loaded as a font", fontdata_filepath.c_str());
                    break;

                case FontType::DIRECT_CFG:
                    if (fontdata_config.has_value()) {
                        ImFontConfig cfg = fontdata_config.value();
                        cfg.SizePixels *= scale;
                        font = fonts->AddFont(&cfg);
                    }
                    if (!font) throw UI_EXCEPTION("Font could not be loaded from the ImFontConfig structure");
                    break;

                case FontType::MEMORY_RAW: {                             // This buffer is transferred to ImGui and then deleted by ImGui
                    // cfg.FontDataOwnedByAtlas is not used to avoid data lifetime issues:
                    void* temp = new uint8_t[fontdata_memdata.size()];   //  just let ImGui do its thing
                    memcpy(temp, fontdata_memdata.data(), fontdata_memdata.size());
                    font = fonts->AddFontFromMemoryTTF(temp, (int)fontdata_memdata.size(), fontdata_size * scale,
                                                       fontdata_config.has_value() ? (&fontdata_config.value()) : nullptr,
                                                       fontdata_glyphs.size() > 1 ? fontdata_glyphs.data() : nullptr);
                    if (!font) throw UI_EXCEPTION("File at '%s' could not be loaded as a font", fontdata_filepath.c_str());
                } break;

                case FontType::MEMORY_COMPRESSED:
                    font = fonts->AddFontFromMemoryCompressedTTF(fontdata_memdata.data(), (int)fontdata_memdata.size(),
                                                                 fontdata_size * scale,
                                                                 fontdata_config.has_value() ? (&fontdata_config.value()) : nullptr,
                                                                 fontdata_glyphs.size() > 1 ? fontdata_glyphs.data() : nullptr);
                    if (!font) throw UI_EXCEPTION("File at '%s' could not be loaded as a font", fontdata_filepath.c_str());
                    break;

                case FontType::RAW:
                    throw UI_EXCEPTION("Cannot reload a raw font!");

                default:
                    throw UI_EXCEPTION("Invalid Font type");
            }
        }

        void applyGlyphs(const std::vector<std::pair<ImWchar, ImWchar>>& glyphs) {
            fontdata_glyphs.clear();
            for (auto [rangeBegin, rangeEnd] : glyphs) {
                fontdata_glyphs.push_back(rangeBegin);
                fontdata_glyphs.push_back(rangeEnd);
            }
            fontdata_glyphs.push_back(0);
        }

        FontType type = FontType::NONE;
        ImFont* font = nullptr;             // Pointer does not own anything, just a reference and thus copy-able

        std::string fontdata_filepath;
        std::string fontdata_memdata;
        float fontdata_size = 0.f;
        std::optional<ImFontConfig> fontdata_config;
        std::vector<ImWchar> fontdata_glyphs;
    };

    inline void ClearFontAtlas() {
        ImGui::GetIO().Fonts->Clear();
    }

    inline void BuildFontAtlas() {
        ImGui::GetIO().Fonts->Build();
    }

    namespace Internal {

        inline static std::unordered_map<std::string, Font> fontMap;
        inline static float fontScale = 1.f;

    }

    inline void AddFontFromFile(const char* fontIdentifier, const std::string& filepath, float size,
                                std::optional<ImFontConfig> fontConfig = std::nullopt,
                                const std::vector<std::pair<ImWchar, ImWchar>>& glyphRanges = {})
    {
        Internal::fontMap[fontIdentifier] = Font(filepath, size, fontConfig, glyphRanges);
    }

    inline void AddFontFromConfig(const char* fontIdentifier, const ImFontConfig& fontConfig)
    {
        Internal::fontMap[fontIdentifier] = Font(fontConfig);
    }

    inline void AddFontFromMemfile(const char* fontIdentifier, const std::string& data, float fontsize,
                                   std::optional<ImFontConfig> fontConfig = std::nullopt,
                                   const std::vector<std::pair<ImWchar, ImWchar>>& glyphRanges = {})
    {
        Internal::fontMap[fontIdentifier] = Font(Font::FontType::MEMORY_RAW, data, fontsize, fontConfig, glyphRanges);
    }

    inline void AddFontFromEmbeddedArray(const char* fontIdentifier, const unsigned int* data, const unsigned int datasize,
                                         float fontsize, std::optional<ImFontConfig> fontConfig = std::nullopt,
                                         const std::vector<std::pair<ImWchar, ImWchar>>& glyphRanges = {})
    {
        std::string datastring;
        datastring.resize(datasize);
        memcpy(datastring.data(), data, datasize);
        Internal::fontMap[fontIdentifier] = Font(Font::FontType::MEMORY_COMPRESSED, datastring, fontsize, fontConfig, glyphRanges);
    }

    inline Font& GetFont(const char* str) {
        try {
            return Internal::fontMap.at(str);
        }
        catch (...) {
            throw UI_EXCEPTION("Cannot access font '%s': No such element, was the font loaded?", str);
        }
    }

    inline std::vector<std::pair<std::string, Font>> GetFonts() {
        std::vector<std::pair<std::string, Font>> fonts;
        for (auto& f : Internal::fontMap) {
            fonts.emplace_back(f);
        }
        return fonts;
    }

    inline void PushFont(const char* str) {
        ImGui::PushFont(GetFont(str));
    }

    inline void PopFont() {
        ImGui::PopFont();
    }

    inline void SetFontScale(float scale) {
        if (scale == Internal::fontScale) return;
        Internal::fontScale = scale;

        ClearFontAtlas();
        for (auto &[key, font]: Internal::fontMap) {
            font.SetScale(Internal::fontScale);
        }
        BuildFontAtlas();

        if (Internal::callbacks.callback_rebuildFontAtlas) {
            Internal::callbacks.callback_rebuildFontAtlas();
        }
        if (Internal::callbacks.callback_requestRedraw) {
            Internal::callbacks.callback_requestRedraw();
        }
    }

    class ScopedFont {
    public:
        explicit ScopedFont(const char* fontIdentifier) {
            ImGui::PushFont(GetFont(fontIdentifier));
        }

        ~ScopedFont() {
            ImGui::PopFont();
        }
    };

}
