#pragma once

#include "BatteryUI/common.h"

namespace BatteryUI {

	struct ButtonStyle;
	struct DropdownStyle;
	struct ContainerStyle;
	struct HorizontalGridStyle;
	struct VerticalGridStyle;

    class PropertyValue;

	namespace Internal {

        extern inline BatteryUI::Callback redrawRequestCallback;
        extern inline std::vector<std::pair<std::string, PropertyValue>> propertyStack;

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
