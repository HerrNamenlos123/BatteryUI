#pragma once

#include "BatteryUI/common.h"
#include "BasicWidget.h"

namespace BatteryUI {
	
	class Button : public BasicWidget {
	public:
        enum class RoundingType {
            NONE,					// Hard corners
            ROUNDED,				// Round corners
            HALF_ROUND,				// Radius is half the button width
        };

		bool clicked = false;		// Single trigger when clicked
		bool held = false;
		bool hovered = false;

		enum RoundingType roundingType = RoundingType::NONE;
		ImGuiBridgeProperty roundingAmount;
		Property<ImVec2> size;
		ColorScheme colors;

		Button() {
			this->name = "Button";
		}

		explicit Button(const std::string& name) {
			this->name = name;
		}

		struct ButtonPreset {
			Style style;
			enum Button::RoundingType roundingType = Button::RoundingType::NONE;
			float roundingAmount = 0.f;
			Property<ImVec2> size;
			ColorScheme colors;
            ButtonPreset() {};
		};

		void loadPreset(struct ButtonPreset preset) {
			style = preset.style;
			roundingType = preset.roundingType;
			//roundingAmount = preset.roundingAmount;
			size = preset.size;
			colors = preset.colors;
		}

		void operator()() {
			draw();
		}

		template <class Archive>
		void load(Archive& ar) {
			EXPORT_ITEM(style);
			EXPORT_ITEM(sameline);
			//EXPORT_ITEM(size);
			EXPORT_ITEM(roundingType);
			//EXPORT_ITEM(roundingAmount);
			EXPORT_ITEM_NESTED(colors, colors);
		}

		template <class Archive>
		void save(Archive& ar) const {
			EXPORT_ITEM(style);
			EXPORT_ITEM(sameline);
			//EXPORT_ITEM(size);
			EXPORT_ITEM(roundingType);
			//EXPORT_ITEM(roundingAmount);
			EXPORT_ITEM_NESTED(colors, colors);
		}

		struct Presets {
			inline static struct ButtonPreset None;			// No special override
			inline static struct ButtonPreset Modern; 		// Modern light-blue
			
			inline static void load() {
				None = Button::ButtonPreset();

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
		void draw();

		ImVec2 actSize;
	};

	struct ButtonDefaultStyle {
		Style style;
		enum Button::RoundingType roundingType = Button::RoundingType::NONE;
		float roundingAmount = 0.f;
		Property<ImVec2> size;
		ColorScheme colors;
		
		ButtonDefaultStyle() = default;
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
			//EXPORT_ITEM(size);
			EXPORT_ITEM(roundingType);
			EXPORT_ITEM(roundingAmount);
			EXPORT_ITEM_NESTED(colors, colors);
		}
	};

    inline void Button::draw() {
		/*bool half_round = false;
		Internal::GetButtonDefaults()->push(half_round);
		style.push();
		colors.push();

		if (half_round)
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, (actSize.x > actSize.y) ? (actSize.y / 2.f) : (actSize.x / 2.f));

		if (roundingType == RoundingType::ROUNDED)
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, roundingAmount);
		else if (roundingType == RoundingType::HALF_ROUND)
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, (actSize.x > actSize.y) ? (actSize.y / 2.f) : (actSize.x / 2.f));

		ImVec2 s = { 0, 0 };
		Internal::GetButtonDefaults()->size.get(s);
		size.get(s);

		if (sameline) ImGui::SameLine();

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

		colors.pop();
		style.pop();
		Internal::GetButtonDefaults()->pop();*/
	}
	
}
