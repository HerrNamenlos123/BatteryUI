#pragma once

#include "BatteryUI/common.h"

namespace BatteryUI {

	struct ButtonStyle;
	struct DropdownStyle;
	struct ContainerStyle;
	struct HorizontalGridStyle;
	struct VerticalGridStyle;

    class PropertyValue;

    struct Config {
        std::function<void(void)> callback_requestRedraw;       // Callback to signal it's time to resume your main loop
                                                                //   and start calling BatteryUI again (after standby),
                                                                //   called from any thread (e.g. from a loading bar
                                                                //   which needs a higher framerate when active)

        std::function<void(void)> callback_rebuildFontAtlas;    // e.g. ImGui_ImplOpenGL3_CreateFontsTexture() or
                                                                //      ImGui::SFML::ImGui::SFML::UpdateFontTexture()
    };

	namespace Internal {

        inline Config callbacks;
        extern inline std::vector<std::pair<std::string, PropertyValue>> propertyStack;  // Cannot be defined yet ('extern')

	}

    inline void RequestRedraw();
    inline void PushProperty(const std::string& property, const PropertyValue& value);
    inline void PopProperty();

    inline std::optional<PropertyValue> RetrieveProperty(const std::string& property);

    template<typename T>
    T RetrieveProperty(const std::string& prop, T defaultValue) {
        auto& value = RetrieveProperty(prop);
        if (value.has_value()) {
            return (T)value.value();
        }
        return defaultValue;
    }
}
