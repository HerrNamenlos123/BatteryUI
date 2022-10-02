#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/Widgets/BasicWidget.h"
#include "BatteryUI/Widgets/Container.h"

namespace BatteryUI {

    struct HorizontalGridStyle {

        Property<float> height;
        Property<bool> borders;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(HorizontalGridStyle, height, borders);
    };

    class HorizontalGrid : public BasicWidget {
    public:
        struct Widget {
            std::function<void(void)> callback;
            Measurement columnWidth;
        };

        std::vector<Widget> elements;
        HorizontalGridStyle style;
        bool sameline = false;

        HorizontalGrid() : BasicWidget("HorizontalGrid") {}
        HorizontalGrid(const std::string& name) : BasicWidget(name) {}
        HorizontalGrid(std::initializer_list<std::function<void(void)>> elements) : BasicWidget("HorizontalGrid") {
            for (auto& element : elements) {
                this->elements.emplace_back(Widget{ element, Measurement("1") });
            }
        }
        HorizontalGrid(std::initializer_list<std::pair<std::function<void(void)>, std::string>> elements) : BasicWidget("HorizontalGrid") {
            for (auto& element : elements) {
                this->elements.emplace_back(Widget{
                    element.first,Measurement(element.second)
                });
            }
        }
        HorizontalGrid(const std::string& name, std::initializer_list<std::function<void(void)>> elements) : BasicWidget(name) {
            for (auto& element : elements) {
                this->elements.emplace_back(Widget{ element, Measurement("1") });
            }
        }
        HorizontalGrid(const std::string& name, std::initializer_list<std::pair<std::function<void(void)>, std::string>> elements)
         : BasicWidget(name) {
            for (auto& element : elements) {
                this->elements.emplace_back(Widget{
                        element.first,Measurement(element.second)
                });
            }
        }

        void operator()(const std::function<void(void)>& callback = nullptr) override {
            calculateWidths();
            ImGui::PushID(getIdentifier().c_str());
            //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

            child.sameline = false;
            for (size_t i = 0; i < elements.size(); i++) {
                float _height = -1.f;
                style.height.get_to(_height);

                UI_PROPERTY_PRIORITY(bool, borders, false, Internal::GetHorizontalGridDefaultStyle()->borders, style.borders);

                child.style.border = borders;
                child.style.size = { widths[i], _height };

                if (sameline)
                    ImGui::SameLine();

                ImGui::PushID(i);
                //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
                child([&] {
                    if (elements[i].callback) {
                        elements[i].callback();      // Call each widget
                    }
                });
                //ImGui::PopStyleVar();
                ImGui::PopID();
                child.sameline = true;
            }
            //ImGui::PopStyleVar();
            ImGui::PopID();
        }

        struct Presets {
            inline static struct HorizontalGridStyle None;			// No special override
            inline static struct HorizontalGridStyle Hidden; 		// No borders
            inline static struct HorizontalGridStyle Framed; 		// with borders

            inline static void load() {
                None = HorizontalGridStyle();
                // No overrides

                Hidden = HorizontalGridStyle();
                Hidden.borders = false;

                Framed = HorizontalGridStyle();
                Framed.borders = true;
            }
        };

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(HorizontalGrid, name, style, sameline);

    private:
        void calculateWidths() {
            size_t num = elements.size();
            widths.resize(num);
            std::fill(widths.begin(), widths.end(), 0.f);

            // First apply all absolute values
            float absSum = 0;
            for (size_t i = 0; i < num; i++) {
                if (elements[i].columnWidth.getUnit() == Measurement::Unit::PIXEL) {
                    widths[i] = elements[i].columnWidth.getValue();// - ImGui::GetStyle().ItemSpacing.x;
                    absSum += elements[i].columnWidth.getValue();
                }
            }

            // If absolutes already fill the page, render immediately
            float available = ImGui::GetContentRegionAvail().x;
            if (absSum >= available) {
                return;
            }

            // Now fill in the relative columns: relative to the page width 0.0 - 1.0
            float relSum = 0.f;
            for (size_t i = 0; i < num; i++) {
                if (elements[i].columnWidth.getUnit() == Measurement::Unit::PERCENT) {
                    float proportion = std::clamp(elements[i].columnWidth.getValue() / 100.f, 0.f, 1.f);
                    widths[i] = available * proportion;// - ImGui::GetStyle().ItemSpacing.x;
                    relSum += available * proportion;
                }
            }

            // Stop if absolute and relative fills the page
            float remaining = std::max(available - absSum - relSum, 0.f);
            if (remaining == 0) {
                return;
            }

            // And finally, everything unitless, NONE is not counted
            // Share the remaining spare among the proportions
            float ulSum = 0.f;
            for (size_t i = 0; i < num; i++) {
                if (elements[i].columnWidth.getUnit() == Measurement::Unit::UNITLESS) {
                    ulSum += elements[i].columnWidth.getValue();
                }
            }
            for (size_t i = 0; i < num; i++) {
                if (elements[i].columnWidth.getUnit() == Measurement::Unit::UNITLESS) {
                    if (ulSum != 0) {
                        float proportion = elements[i].columnWidth.getValue() / ulSum;
                        widths[i] = remaining * proportion;// - ImGui::GetStyle().ItemSpacing.x;
                    }
                }
            }
        }

    private:
        Container child;
        std::vector<float> widths;
    };

}