#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/RootUI.h"

#include "BatteryUI/Widgets/Button.h"
#include "BatteryUI/Widgets/Dropdown.h"

namespace BatteryUI {
	namespace Internal {

        inline BatteryUI::Callback redrawRequestCallback;
        inline std::vector<std::pair<std::string, PropertyValue>> propertyStack;

	}

    inline void RequestRedraw() {
        if (Internal::redrawRequestCallback)
            Internal::redrawRequestCallback();
    }

    inline void PushProperty(const std::string& property, const PropertyValue& value) {
        Internal::propertyStack.emplace_back(property, value);

        auto stylevar = magic_enum::enum_cast<ImGuiStyleVar_>(property);
        if (stylevar.has_value()) {
            switch (value.getUnit()) {
                case PropertyValue::Unit::UNITLESS:
                case PropertyValue::Unit::PIXEL:
                case PropertyValue::Unit::PERCENT:
                    ImGui::PushStyleVar(stylevar.value(), (float)value);
                    break;

                case PropertyValue::Unit::VEC2:
                    ImGui::PushStyleVar(stylevar.value(), (ImVec2)value);
                    break;

                default:
                    throw UI_EXCEPTION("Cannot push style color %s: Property value is not float or ImVec2!", property.c_str());
            }
            return;
        }

        auto stylecol = magic_enum::enum_cast<ImGuiCol_>(property);
        if (stylecol.has_value()) {
            switch (value.getUnit()) {
                case PropertyValue::Unit::COLOR_HEX:
                    ImGui::PushStyleColor(stylecol.value(), (ImVec4)value / 255);
                    break;

                default:
                    throw UI_EXCEPTION("Cannot push style color %s: Property value is not COLOR_HEX!", property.c_str());
            }
            return;
        }

        // None of the above apply (Not an ImGuiStyleVar_)
        if (property == "ImGuiItemWidth") {
            ImGui::PushItemWidth((float)value);
        }
    }

    inline void PopProperty() {
        if (Internal::propertyStack.empty())
            throw UI_EXCEPTION("Cannot pop property from global style stack: Stack is empty! Probably a style push/pop mismatch!");

        auto& [property, value] = Internal::propertyStack.back();

        auto stylevar = magic_enum::enum_cast<ImGuiStyleVar_>(property);
        if (stylevar.has_value()) {
            switch (value.getUnit()) {
                case PropertyValue::Unit::UNITLESS:
                case PropertyValue::Unit::PIXEL:
                case PropertyValue::Unit::PERCENT:
                case PropertyValue::Unit::VEC2:
                    ImGui::PopStyleVar();
                    break;

                default:
                    throw UI_EXCEPTION("Cannot pop style color %s: Property value is not float or ImVec2!", property.c_str());
            }
        }

        auto stylecol = magic_enum::enum_cast<ImGuiCol_>(property);
        if (stylecol.has_value()) {
            switch (value.getUnit()) {
                case PropertyValue::Unit::COLOR_HEX:
                    ImGui::PopStyleColor();
                    break;

                default:
                    throw UI_EXCEPTION("Cannot pop style color %s: Property value is not COLOR_HEX!", property.c_str());
            }
        }

        // None of the above apply (Not an ImGuiStyleVar_)
        if (property == "ImGuiItemWidth") {
            ImGui::PushItemWidth((float)value);
        }

        Internal::propertyStack.pop_back();
    }

    inline std::optional<PropertyValue> RetrieveProperty(const std::string& property) {
        for (auto iter = Internal::propertyStack.rbegin(); iter != Internal::propertyStack.rend(); ++iter) {
            auto& [_property, _value] = *iter;
            if (_property == property) {
                return _value;
            }
        }
        return std::nullopt;
    }
}
