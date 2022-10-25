#pragma once

#include "BatteryUI/common.h"
#include "BasicWidget.h"

namespace BatteryUI {
	
	class Button : public BasicWidget {
	public:

		bool clicked = false;		// Single trigger when clicked
		bool held = false;
		bool hovered = false;

        bool sameline = false;
        WidgetStyle style;

		Button() : BasicWidget("Button") {}
		explicit Button(const std::string& name) : BasicWidget(name) {}

		void operator()(const BatteryUI::Callback& callback = nullptr) override {

            if (sameline)
                ImGui::SameLine();

            style.push();
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, RetrieveProperty("ButtonBorderRadius", 0.f));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, RetrieveProperty("ButtonBorderWidth", 1.f));
            ImGui::PushStyleColor(ImGuiCol_Border, RetrieveProperty("ButtonBorderColor", ImVec4(255, 255, 255, 255)) / 255);
            ImGui::PushStyleColor(ImGuiCol_Button, RetrieveProperty("ButtonBaseColor", ImVec4(255, 255, 255, 255)) / 255);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, RetrieveProperty("ButtonHoverColor", ImVec4(255, 255, 255, 255)) / 255);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, RetrieveProperty("ButtonActiveColor", ImVec4(255, 255, 255, 255)) / 255);

            float width = RetrieveProperty("ButtonWidth", 0.f);
            float height = RetrieveProperty("ButtonHeight", 0.f);

            clicked = ImGui::Button(getIdentifier().c_str(), { width, height });
            held = ImGui::IsItemActive();
            hovered = ImGui::IsItemHovered();

            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::PopStyleVar();
            style.pop();

            if (callback && clicked)
                callback();
		}

        BATTERYUI_SERIALIZE(Button, name, sameline);
	};
	
}
