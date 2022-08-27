#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/Style.h"
#include "BatteryUI/InternalDecl.h"
#include "BatteryUI/BasicWidget.h"

namespace BatteryUI {
	
	class Button : public BasicWidget {
	public:
		bool clicked = false;		// This is only true for one frame
		bool held = false;
		bool hovered = false;

		enum class RoundingType {
			NONE,					// Hard corners
			ROUNDED,				// Round corners
			HALF_ROUND,				// Radius is half the button width
		};
		enum class RoundingType roundingType = RoundingType::NONE;
		float roundingAmount = 0.f;
		vec2_opt size;
		ColorScheme colors;

		Button() {
			this->name = "Button";
		}

		Button(const std::string& name) {
			this->name = name;
		}

		void draw() {
			bool half_round = false;
			vec2_opt defaultSize;
			style.push();
			colors.push();
			Internal::PushButtonDefaultStyle(half_round);

			if (half_round)
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, (actSize.x > actSize.y) ? (actSize.y / 2.f) : (actSize.x / 2.f));

			if (roundingType == RoundingType::ROUNDED)
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, roundingAmount);
			else if (roundingType == RoundingType::HALF_ROUND)
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, (actSize.x > actSize.y) ? (actSize.y / 2.f) : (actSize.x / 2.f));

			vec2 s = { 0, 0 };
			if (Internal::GetButtonDefaultSize().has_value())
				s = Internal::GetButtonDefaultSize().value();
			if (size.has_value())
				s = size.value();
			
			clicked = ImGui::Button(getIdentifier().c_str(), s);
			actSize = ImGui::GetItemRectSize();
			held = ImGui::IsItemActive();
			hovered = ImGui::IsItemHovered();
			
			if (roundingType == RoundingType::ROUNDED)
				ImGui::PopStyleVar();
			else if (roundingType == RoundingType::HALF_ROUND)
				ImGui::PopStyleVar();
			
			if (half_round)
				ImGui::PopStyleVar();

			Internal::PopButtonDefaultStyle();
			colors.pop();
			style.pop();
		}

		struct ButtonPreset {
			Style style;
			enum class Button::RoundingType roundingType = Button::RoundingType::NONE;
			float roundingAmount = 0.f;
			vec2_opt size;
			ColorScheme colors;
		};

		void loadPreset(struct ButtonPreset preset) {
			style = preset.style;
			roundingType = preset.roundingType;
			roundingAmount = preset.roundingAmount;
			size = preset.size;
			colors = preset.colors;
		}

		template <class Archive>
		void load(Archive& ar) {
			EXPORT_ITEM(style);
			EXPORT_ITEM(size);
			EXPORT_ITEM(roundingType);
			EXPORT_ITEM(roundingAmount);
			EXPORT_ITEM_NESTED(colors, colors);
		}

		template <class Archive>
		void save(Archive& ar) const {
			EXPORT_ITEM(style);
			EXPORT_ITEM(size);
			EXPORT_ITEM(roundingType);
			EXPORT_ITEM(roundingAmount);
			EXPORT_ITEM_NESTED(colors, colors);
		}

		struct Presets {
			inline static struct ButtonPreset Standard;		// Standard ImGui-look
			inline static struct ButtonPreset Modern; 		// Modern light-blue
			
			inline static void load() {
				Standard = Button::ButtonPreset();

				Modern = Button::ButtonPreset();
				Modern.colors.addColor("ImGuiCol_Button", 0, 0.6, 0.9, 0.84);
				Modern.colors.addColor("ImGuiCol_ButtonHovered", 0.1, 0.7, 1.0, 0.84);
				Modern.colors.addColor("ImGuiCol_ButtonActive", 0, 0.5, 0.8, 0.84);
				Modern.size = { 160, 40 };
				Modern.roundingType = Button::RoundingType::ROUNDED;
				Modern.roundingAmount = 5;
			}
		};

	private:
		vec2 actSize;
	};

	struct ButtonDefaultStyle {
		Style style;
		enum class Button::RoundingType roundingType = Button::RoundingType::NONE;
		float roundingAmount = 0.f;
		vec2_opt size;
		ColorScheme colors;
		
		ButtonDefaultStyle() {}
		ButtonDefaultStyle(struct Button::ButtonPreset preset) {
			loadPreset(preset);
		}

		void push(bool& half_round) {
			style.push();
			colors.push();
			if (roundingType == Button::RoundingType::ROUNDED)
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, roundingAmount);
			else if (roundingType == Button::RoundingType::HALF_ROUND)
				half_round = true;
		}

		void pop() {
			if (roundingType == Button::RoundingType::ROUNDED)
				ImGui::PopStyleVar();
			else if (roundingType == Button::RoundingType::HALF_ROUND)
				; // Nothing
			colors.pop();
			style.pop();
		}

		void loadPreset(struct Button::ButtonPreset preset) {
			style = preset.style;
			roundingType = preset.roundingType;
			roundingAmount = preset.roundingAmount;
			size = preset.size;
			colors = preset.colors;
		}

		template <class Archive>
		void serialize(Archive& ar) {
			EXPORT_ITEM(style);
			EXPORT_ITEM(size);
			EXPORT_ITEM(roundingType);
			EXPORT_ITEM(roundingAmount);
			EXPORT_ITEM_NESTED(colors, colors);
		}
	};
	
}
