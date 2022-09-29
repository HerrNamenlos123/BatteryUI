#pragma once

#include "BatteryUI/common.h"
#include "BasicWidget.h"

namespace BatteryUI {

    struct ButtonStyle {
        Property<ImVec2> size;
        SameLine sameLine;

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

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(ButtonStyle, size, sameLine, rounding, color, colorHovered, colorActive);
    };
	
	class Button : public BasicWidget {
	public:

		bool clicked = false;		// Single trigger when clicked
		bool held = false;
		bool hovered = false;

        ButtonStyle style;

		Button() : BasicWidget("Button") {}
		explicit Button(const std::string& name) : BasicWidget(name) {}

		void operator()() {
			draw();
		}

		struct Presets {
			inline static struct ButtonStyle None;			// No special override
			inline static struct ButtonStyle Modern; 		// Modern light-blue
			
			inline static void load() {
				None = ButtonStyle();
                // No overrides

				Modern = ButtonStyle();
                Modern.size = { 160, 40 };
				Modern.color = ImVec4(0, 0.6, 0.9, 0.84);
				Modern.colorHovered = ImVec4(0.1, 0.7, 1.0, 0.84);
				Modern.colorActive = ImVec4(0, 0.5, 0.8, 0.84);
				Modern.rounding = { RoundingType::ROUNDED, 5 };
			}
		};

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Button, style);

	private:
		void draw();
	};

    inline void Button::draw() {
		Internal::GetButtonDefaults()->push();
        style.push();

		ImVec2 _size = { 0, 0 };                        // ImGui default is the lowest priority
		Internal::GetButtonDefaults()->size.get(_size);   // Then the global default
		style.size.get(_size);                            // And then the button itself

        style.sameLine();

		clicked = ImGui::Button(getIdentifier().c_str(), _size);
		held = ImGui::IsItemActive();
		hovered = ImGui::IsItemHovered();

        style.pop();
		Internal::GetButtonDefaults()->pop();
	}
	
}
