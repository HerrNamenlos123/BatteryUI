#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/Widgets/BasicWidget.h"
#include "BatteryUI/Widgets/Container.h"

namespace BatteryUI {

    struct VerticalGridStyle {

        Property<float> width;
        Property<bool> borders;

        BATTERYUI_SERIALIZE(VerticalGridStyle, width, borders);
    };

    class VerticalGrid : public BasicWidget {
    public:
        struct Widget {
            BatteryUI::Callback callback;
            Measurement rowHeight;
        };

        std::vector<Widget> elements;
        VerticalGridStyle style;
        bool sameline = false;

        VerticalGrid() : BasicWidget("VerticalGrid") {}
        VerticalGrid(const std::string& name) : BasicWidget(name) {}
        VerticalGrid(std::initializer_list<BatteryUI::Callback> elements) : BasicWidget("VerticalGrid") {
            for (auto& element : elements) {
                this->elements.emplace_back(Widget{ element, Measurement("1") });
            }
        }
        VerticalGrid(std::initializer_list<std::pair<BatteryUI::Callback, std::string>> elements) : BasicWidget("VerticalGrid") {
            for (auto& element : elements) {
                this->elements.emplace_back(Widget{
                        element.first,Measurement(element.second)
                });
            }
        }
        VerticalGrid(const std::string& name, std::initializer_list<BatteryUI::Callback> elements) : BasicWidget(name) {
            for (auto& element : elements) {
                this->elements.emplace_back(Widget{ element, Measurement("1") });
            }
        }
        VerticalGrid(const std::string& name, std::initializer_list<std::pair<BatteryUI::Callback, std::string>> elements)
                : BasicWidget(name) {
            for (auto& element : elements) {
                this->elements.emplace_back(Widget{
                        element.first,Measurement(element.second)
                });
            }
        }

        void operator()(const BatteryUI::Callback& callback = nullptr) override {
            calculateHeights();
            ImGui::PushID(getIdentifier().c_str());

            for (size_t i = 0; i < elements.size(); i++) {
                float _width = -1.f;
                style.width.get_to(_width);

                UI_PROPERTY_PRIORITY(bool, borders, false, Internal::GetVerticalGridDefaultStyle()->borders, style.borders);

                child.style.border = borders;
                child.style.size = { _width, heights[i] };
                ImGui::PushID(i);
                child([&] {
                    if (elements[i].callback) {
                        elements[i].callback();      // Call each widget
                    }
                });
                ImGui::PopID();
            }
            ImGui::PopID();
        }

        struct Presets {
            inline static struct VerticalGridStyle None;			// No special override
            inline static struct VerticalGridStyle Hidden; 		// No borders
            inline static struct VerticalGridStyle Framed; 		// with borders

            inline static void load() {
                None = VerticalGridStyle();
                // No overrides

                Hidden = VerticalGridStyle();
                Hidden.borders = false;

                Framed = VerticalGridStyle();
                Framed.borders = true;
            }
        };

        BATTERYUI_SERIALIZE(VerticalGrid, name, style, sameline);

    private:
        void calculateHeights() {
            size_t num = elements.size();
            heights.resize(num);
            std::fill(heights.begin(), heights.end(), 0.f);

            // First apply all absolute values
            float absSum = 0;
            for (size_t i = 0; i < num; i++) {
                if (elements[i].rowHeight.getUnit() == Measurement::Unit::PIXEL) {
                    heights[i] = elements[i].rowHeight.getValue();// - ImGui::GetStyle().ItemSpacing.y;
                    absSum += elements[i].rowHeight.getValue();
                }
            }

            // If absolutes already fill the page, render immediately
            float available = ImGui::GetContentRegionAvail().y;
            if (absSum >= available) {
                return;
            }

            // Now fill in the relative columns: relative to the page width 0.0 - 1.0
            float relSum = 0.f;
            for (size_t i = 0; i < num; i++) {
                if (elements[i].rowHeight.getUnit() == Measurement::Unit::PERCENT) {
                    float proportion = std::clamp(elements[i].rowHeight.getValue() / 100.f, 0.f, 1.f);
                    heights[i] = available * proportion;//- ImGui::GetStyle().ItemSpacing.y;
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
                if (elements[i].rowHeight.getUnit() == Measurement::Unit::UNITLESS) {
                    ulSum += elements[i].rowHeight.getValue();
                }
            }
            for (size_t i = 0; i < num; i++) {
                if (elements[i].rowHeight.getUnit() == Measurement::Unit::UNITLESS) {
                    if (ulSum != 0) {
                        float proportion = elements[i].rowHeight.getValue() / ulSum;
                        heights[i] = remaining * proportion;// - ImGui::GetStyle().ItemSpacing.y;
                    }
                }
            }
        }

    private:
        Container child;
        std::vector<float> heights;
    };

}