#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/magic_enum.h"

namespace BatteryUI {

	struct ImGuiColorScheme {

		struct ImGuiColor {
			std::string name;
			ImVec4 color;
		};

		std::vector<ImGuiColor> colors;

		ImGuiColor& operator[](size_t index) {
			return colors[index];
		}

		size_t size() const {
			return colors.size();
		}
	};

	inline ImGuiColorScheme RetrieveColorScheme() {
		ImGuiColorScheme scheme;

		for (size_t i = 0; i < ImGuiCol_COUNT; i++) {
			std::string name = magic_enum::enum_name((ImGuiCol_)i).data();
			scheme.colors.push_back({ name, ImGui::GetStyle().Colors[i] });
		}
		
		return scheme;
	}

	inline void ApplyColorScheme(const ImGuiColorScheme& scheme) {
		ImVec4* colors = ImGui::GetStyle().Colors;

		for (size_t i = 0; i < scheme.size() && i < ImGuiCol_COUNT; i++) {
			colors[i] = scheme.colors[i].color;
		}
	}
	
	/*
	class Style {
	public:
		Style() {}

	private:
		std::vector<std::string>
	};*/
	
}
