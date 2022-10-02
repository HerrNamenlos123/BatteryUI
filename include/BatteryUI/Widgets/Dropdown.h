#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/Widgets/BasicWidget.h"

namespace BatteryUI {

    struct DropdownStyle {
        Rounding rounding;
        Property<float> width;
        ImGuiPropVec4<ImGuiCol_Button> color;
        ImGuiPropVec4<ImGuiCol_ButtonHovered> colorHovered;
        ImGuiPropVec4<ImGuiCol_ButtonActive> colorActive;
    };

	class Dropdown : public BasicWidget {
	public:
		std::vector<std::string> items;
		size_t selectedItem = -1;			// The std::vector index of the selected item (-1 if invalid)

        DropdownStyle style;

		Dropdown() : BasicWidget("Dropdown") {}
		explicit Dropdown(const std::string& name) : BasicWidget(name) {}

		void operator()(const std::function<void(void)>& callback = nullptr) {
			draw();
		}

		struct Presets {
			inline static struct DropdownStyle None;			// No special override
			inline static struct DropdownStyle Modern; 		// Modern light-blue

			inline static void load() {
				None = DropdownStyle();

				Modern = DropdownStyle();
				Modern.color = ImVec4(0, 0.6, 0.9, 0.84);
				Modern.colorHovered = ImVec4(0.1, 0.7, 1.0, 0.84);
				Modern.colorActive = ImVec4(0, 0.5, 0.8, 0.84);
				Modern.width = 0;
				Modern.rounding = { RoundingType::ROUNDED, 5 };
            }
		};

	private:
		void draw();

		ImVec2 actSize;
	};

	inline void Dropdown::draw() {
		/*bool half_round = false;
		Internal::GetDropdownDefaults()->push(half_round);
		style.push();
		colors.push();

		if (selectedItem == -1 && items.size() > 0)
			selectedItem = 0;
		if (selectedItem >= items.size())
			selectedItem = items.size() - 1;
		if (items.empty())
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

		std::string label = (items.empty()) ? "" : items[selectedItem];
		ImGui::PushID((int)id);
		ImGui::SetNextItemWidth(w);
		if (ImGui::BeginCombo(getIdentifier().c_str(), label.c_str())) {
			if (!items.empty()) {
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
		Internal::GetDropdownDefaults()->pop();*/
	}

}
