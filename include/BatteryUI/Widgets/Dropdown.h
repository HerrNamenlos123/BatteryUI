#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/BasicWidget.h"

namespace BatteryUI {

	class Dropdown : public BasicWidget {
	public:
		std::vector<std::string> items;
		size_t selectedItem = -1;			// The std::vector index of the selected item (-1 if invalid)

		enum class RoundingType {
			NONE,					// Hard corners
			ROUNDED,				// Round corners
			HALF_ROUND,				// Radius is half the button width
		};
		enum class RoundingType roundingType = RoundingType::NONE;
		float roundingAmount = 0.f;
		std::optional<float> width;
		ColorScheme colors;

		Dropdown() {
			this->name = "DropDown";
		}

		Dropdown(const std::string& name) {
			this->name = name;
		}

		struct DropdownPreset {
			Style style;
			enum class Dropdown::RoundingType roundingType = Dropdown::RoundingType::NONE;
			float roundingAmount = 0.f;
			std::optional<float> width;
			ColorScheme colors;
		};

		void loadPreset(struct DropdownPreset preset) {
			style = preset.style;
			roundingType = preset.roundingType;
			roundingAmount = preset.roundingAmount;
			width = preset.width;
			colors = preset.colors;
		}

		void operator()() {
			draw();
		}

		template <class Archive>
		void load(Archive& ar) {
			EXPORT_ITEM(style);
			EXPORT_ITEM(sameline);
			EXPORT_ITEM(width);
			EXPORT_ITEM(roundingType);
			EXPORT_ITEM(roundingAmount);
			EXPORT_ITEM_NESTED(colors, colors);
		}

		template <class Archive>
		void save(Archive& ar) const {
			EXPORT_ITEM(style);
			EXPORT_ITEM(sameline);
			EXPORT_ITEM(width);
			EXPORT_ITEM(roundingType);
			EXPORT_ITEM(roundingAmount);
			EXPORT_ITEM_NESTED(colors, colors);
		}

		struct Presets {
			inline static struct DropdownPreset None;			// No special override
			inline static struct DropdownPreset Modern; 		// Modern light-blue

			inline static void load() {
				None = Dropdown::DropdownPreset();

				Modern = Dropdown::DropdownPreset();
				//Modern.colors.addColor("ImGuiCol_Button", 0, 0.6, 0.9, 0.84);
				//Modern.colors.addColor("ImGuiCol_ButtonHovered", 0.1, 0.7, 1.0, 0.84);
				//Modern.colors.addColor("ImGuiCol_ButtonActive", 0, 0.5, 0.8, 0.84);
				//Modern.width = 0;
				//Modern.roundingType = Dropdown::RoundingType::ROUNDED;
				//Modern.roundingAmount = 5;
			}
		};

	private:
		void draw();

		vec2 actSize;
	};

	struct DropdownDefaultStyle {
		Style style;
		enum class Dropdown::RoundingType roundingType = Dropdown::RoundingType::NONE;
		float roundingAmount = 0.f;
		std::optional<float> width;
		ColorScheme colors;

		DropdownDefaultStyle() {}
		DropdownDefaultStyle(struct Dropdown::DropdownPreset preset) {
			loadPreset(preset);
		}

		void push(bool& half_round) {
			style.push();
			colors.push();
			if (roundingType == Dropdown::RoundingType::ROUNDED)
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, roundingAmount);
			else if (roundingType == Dropdown::RoundingType::HALF_ROUND)
				half_round = true;
		}

		void pop() {
			if (roundingType == Dropdown::RoundingType::ROUNDED)
				ImGui::PopStyleVar();
			else if (roundingType == Dropdown::RoundingType::HALF_ROUND)
				; // Nothing
			colors.pop();
			style.pop();
		}

		void loadPreset(struct Dropdown::DropdownPreset preset) {
			style = preset.style;
			roundingType = preset.roundingType;
			roundingAmount = preset.roundingAmount;
			width = preset.width;
			colors = preset.colors;
		}

		template <class Archive>
		void load(Archive& ar) {
			EXPORT_ITEM(style);
			LOAD_ITEM_IF_EXISTS_OPT(float, width);
			EXPORT_ITEM(roundingType);
			EXPORT_ITEM(roundingAmount);
			EXPORT_ITEM_NESTED(colors, colors);
		}

		template <class Archive>
		void save(Archive& ar) const {
			EXPORT_ITEM(style);
			SAVE_ITEM_IF_VALUE_OPT(float, width);
			EXPORT_ITEM(roundingType);
			EXPORT_ITEM(roundingAmount);
			EXPORT_ITEM_NESTED(colors, colors);
		}
	};

	inline void Dropdown::draw() {
		bool half_round = false;
		Internal::GetDropdownDefaults()->push(half_round);
		style.push();
		colors.push();

		if (selectedItem == -1 && items.size() > 0)
			selectedItem = 0;
		if (selectedItem >= items.size())
			selectedItem = items.size() - 1;
		if (items.size() == 0)
			selectedItem = -1;

		if (half_round)
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, (actSize.x > actSize.y) ? (actSize.y / 2.f) : (actSize.x / 2.f));

		if (roundingType == RoundingType::ROUNDED)
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, roundingAmount);
		else if (roundingType == RoundingType::HALF_ROUND)
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, (actSize.x > actSize.y) ? (actSize.y / 2.f) : (actSize.x / 2.f));

		float w = ImGui::GetFontSize() * 10.f;
		if (Internal::GetDropdownDefaults()->width.has_value())
			w = Internal::GetDropdownDefaults()->width.value();
		if (width.has_value())
			w = width.value();

		if (sameline) ImGui::SameLine();

		std::string& label = (items.size() == 0) ? "" : items[selectedItem];
		ImGui::PushID(id);
		ImGui::SetNextItemWidth(w);
		if (ImGui::BeginCombo(getIdentifier().c_str(), label.c_str())) {
			if (items.size() != 0) {
				for (size_t i = 0; i < items.size(); i++) {
					ImGui::PushID(i);
					bool is_selected = (i == selectedItem);
					if (ImGui::Selectable(items[i].c_str(), is_selected))
						selectedItem = i;
					if (is_selected)
						ImGui::SetItemDefaultFocus();
					ImGui::PopID();
				}
			}
			ImGui::EndCombo();
		}
		ImGui::PopID();

		actSize = ImGui::GetItemRectSize();

		if (roundingType == RoundingType::ROUNDED)
			ImGui::PopStyleVar();
		else if (roundingType == RoundingType::HALF_ROUND)
			ImGui::PopStyleVar();

		if (half_round)
			ImGui::PopStyleVar();

		colors.pop();
		style.pop();
		Internal::GetDropdownDefaults()->pop();
	}

}
