#pragma once

#include "BatteryUI/common.h"
#include "BasicWidget.h"

namespace BatteryUI {

    struct ButtonStyle {
        Property<ImVec2> size;

        Rounding rounding;
        ImGuiPropVec4<ImGuiCol_Button> color;
        ImGuiPropVec4<ImGuiCol_ButtonHovered> colorHovered;
        ImGuiPropVec4<ImGuiCol_ButtonActive> colorActive;

        void push() {
            rounding.push();
            color.push();
            colorHovered.push();
            colorActive.push();
        }

        void pop() {
            colorActive.pop();
            colorHovered.pop();
            color.pop();
            rounding.pop();
        }

        BATTERYUI_SERIALIZE(ButtonStyle, size, rounding, color, colorHovered, colorActive);
    };
	
	class Button : public BasicWidget {
	public:

		bool clicked = false;		// Single trigger when clicked
		bool held = false;
		bool hovered = false;

        ButtonStyle style;
        bool sameline = false;

		Button() : BasicWidget("Button") {}
		explicit Button(const std::string& name) : BasicWidget(name) {}

		void operator()(const BatteryUI::Callback& callback = nullptr) override {

            UI_PROPERTY_PRIORITY(ImVec2, size, ImVec2(0, 0), Internal::GetButtonDefaultStyle()->size, style.size);

            if (sameline)
                ImGui::SameLine();

            Internal::GetButtonDefaultStyle()->push();
            style.push();

            clicked = ImGui::Button(getIdentifier().c_str(), size);
            held = ImGui::IsItemActive();
            hovered = ImGui::IsItemHovered();

            if (callback && clicked)
                callback();

            style.pop();
            Internal::GetButtonDefaultStyle()->pop();
		}

		struct Presets {
			inline static struct ButtonStyle None;			// No special override
			inline static struct ButtonStyle Modern; 		// Modern light-blue
			
			inline static void load() {
				None = ButtonStyle();
                // No overrides

				Modern = ButtonStyle();
                Modern.size = { -1, 40 };
				Modern.color = ImVec4(0, 0.6, 0.9, 0.84);
				Modern.colorHovered = ImVec4(0.1, 0.7, 1.0, 0.84);
				Modern.colorActive = ImVec4(0, 0.5, 0.8, 0.84);
				Modern.rounding = { RoundingType::ROUNDED, 5 };
			}
		};

        BATTERYUI_SERIALIZE(Button, name, style, sameline);
	};
	
}
