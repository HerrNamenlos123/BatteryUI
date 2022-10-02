#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/Widgets/BasicWidget.h"

namespace BatteryUI {

    struct ChildStyle {
        Property<ImVec2> size;
        Property<bool> border;

        ImGuiPropVec2<ImGuiStyleVar_WindowPadding> padding;

        void push() {
            padding.push();
        }

        void pop() {
            padding.pop();
        }

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(ChildStyle, size, border, padding);
    };

    class Child : public BasicWidget {
    public:

        ChildStyle style;
        bool sameline = false;
        ImGuiWindowFlags_ flags = ImGuiWindowFlags_None;

        Child() : BasicWidget("Child") {}
        Child(const std::string& name) : BasicWidget(name) {}

        void operator()(const std::function<void(void)>& callback) override {

            UI_PROPERTY_PRIORITY(ImVec2, size, ImVec2(0, 0), Internal::GetChildDefaultStyle()->size, style.size);
            UI_PROPERTY_PRIORITY(bool, border, false, Internal::GetChildDefaultStyle()->border, style.border);

            if (sameline)
                ImGui::SameLine();

            Internal::GetChildDefaultStyle()->push();
            style.push();
            ImGui::BeginChild(getIdentifier().c_str(), size, border, flags);

            if (callback)
                callback();

            ImGui::EndChild();
            style.pop();
            Internal::GetChildDefaultStyle()->pop();
        }

        struct Presets {
            inline static struct ChildStyle None;			// No special override
            inline static struct ChildStyle Thin; 		    // No padding

            inline static void load() {
                None = ChildStyle();
                // No overrides

                Thin = ChildStyle();
                Thin.padding = { 0, 0 };
            }
        };

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Child, name, style, sameline, flags);
    };

}