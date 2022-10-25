#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/Widgets/BasicWidget.h"

namespace BatteryUI {

    class Container : public BasicWidget {
    public:

        WidgetStyle style;
        ImVec2 size = { -1, -1 };
        bool sameline = false;
        ImGuiWindowFlags_ flags = ImGuiWindowFlags_None;

        Container() : BasicWidget("Container") {}
        Container(const std::string& name) : BasicWidget(name) {}

        void operator()(const BatteryUI::Callback& callback) override {
            ImVec2 spacing = ImGui::GetStyle().ItemSpacing;
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

            if (sameline)
                ImGui::SameLine();

            style.push();
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, RetrieveProperty("ContainerBorderRadius", 0.f));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, RetrieveProperty("ContainerBorderWidth", 0.f));
            ImGui::PushStyleColor(ImGuiCol_Border, RetrieveProperty("ContainerBorderColor", ImVec4(255, 255, 255, 255)) / 255);

            float border = RetrieveProperty("ContainerBorderWidth", 0.f);

            ImGui::BeginChild(getIdentifier().c_str(), size, border > 0.f, flags);
            if (callback) {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, spacing);
                callback();
                ImGui::PopStyleVar();
            }
            ImGui::EndChild();

            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
            ImGui::PopStyleVar();
            style.pop();

            ImGui::PopStyleVar();
        }

        BATTERYUI_SERIALIZE(Container, name, sameline, flags);
    };

}