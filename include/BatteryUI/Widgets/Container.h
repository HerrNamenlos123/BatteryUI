#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/Widgets/BasicWidget.h"

namespace BatteryUI {

    struct ContainerStyle {
        Property<ImVec2> size;
        Property<bool> border;

        ImGuiPropVec2<ImGuiStyleVar_WindowPadding> padding;

        void push() {
            padding.push();
        }

        void pop() {
            padding.pop();
        }

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(ContainerStyle, size, border, padding);
    };

    class Container : public BasicWidget {
    public:

        ContainerStyle style;
        bool sameline = false;
        ImGuiWindowFlags_ flags = ImGuiWindowFlags_None;

        Container() : BasicWidget("Container") {}
        Container(const std::string& name) : BasicWidget(name) {}

        void operator()(const BatteryUI::Callback& callback) override {
            ImVec2 spacing = ImGui::GetStyle().ItemSpacing;
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

            UI_PROPERTY_PRIORITY(ImVec2, size, ImVec2(0, 0), Internal::GetChildDefaultStyle()->size, style.size);
            UI_PROPERTY_PRIORITY(bool, border, false, Internal::GetChildDefaultStyle()->border, style.border);

            if (sameline)
                ImGui::SameLine();

            Internal::GetChildDefaultStyle()->push();
            style.push();
            ImGui::BeginChild(getIdentifier().c_str(), size, border, flags);

            if (callback) {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, spacing);
                callback();
                ImGui::PopStyleVar();
            }

            ImGui::EndChild();
            style.pop();
            Internal::GetChildDefaultStyle()->pop();
            ImGui::PopStyleVar();
        }

        struct Presets {
            inline static struct ContainerStyle None;			// No special override
            inline static struct ContainerStyle Thin; 		    // No padding

            inline static void load() {
                None = ContainerStyle();
                // No overrides

                Thin = ContainerStyle();
                Thin.padding = { 0, 0 };
            }
        };

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Container, name, style, sameline, flags);
    };

}