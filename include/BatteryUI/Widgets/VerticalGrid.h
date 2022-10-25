#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/Widgets/BasicWidget.h"
#include "BatteryUI/Widgets/Container.h"

namespace BatteryUI {

    class VerticalGrid : public BasicWidget {
    public:
        struct Widget {
            BatteryUI::Callback callback;
            PropertyValue rowHeight;
        };

        std::vector<Widget> elements;
        WidgetStyle style;
        float width = -1;
        bool sameline = false;

        VerticalGrid() : BasicWidget("VerticalGrid") {}
        VerticalGrid(const std::string& name) : BasicWidget(name) {}
        VerticalGrid(std::initializer_list<BatteryUI::Callback> elements) : BasicWidget("VerticalGrid") {
            for (auto& element : elements) {
                this->elements.emplace_back(Widget{element, PropertyValue("1") });
            }
        }
        VerticalGrid(std::initializer_list<std::pair<BatteryUI::Callback, std::string>> elements) : BasicWidget("VerticalGrid") {
            for (auto& element : elements) {
                this->elements.emplace_back(Widget{
                        element.first, PropertyValue(element.second)
                });
            }
        }
        VerticalGrid(const std::string& name, std::initializer_list<BatteryUI::Callback> elements) : BasicWidget(name) {
            for (auto& element : elements) {
                this->elements.emplace_back(Widget{element, PropertyValue("1") });
            }
        }
        VerticalGrid(const std::string& name, std::initializer_list<std::pair<BatteryUI::Callback, std::string>> elements)
                : BasicWidget(name) {
            for (auto& element : elements) {
                this->elements.emplace_back(Widget{
                        element.first, PropertyValue(element.second)
                });
            }
        }

        void operator()(const BatteryUI::Callback& callback = nullptr) override {
            calculateHeights();
            ImGui::PushID(getIdentifier().c_str());

            if (sameline)
                ImGui::SameLine();

            for (size_t i = 0; i < elements.size(); i++) {

                child.style["ContainerBorderColor"] = RetrieveProperty("GridBorderColor", ImVec4(255, 255, 255, 255) / 255);
                child.style["ContainerBorderWidth"] = RetrieveProperty("GridBorderWidth", 0.f);

                child.size = { width, heights[i] };

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

        BATTERYUI_SERIALIZE(VerticalGrid, name, sameline);

    private:
        void calculateHeights() {
            size_t num = elements.size();
            heights.resize(num);
            std::fill(heights.begin(), heights.end(), 0.f);

            // First apply all absolute values
            float absSum = 0;
            for (size_t i = 0; i < num; i++) {
                if (elements[i].rowHeight.getUnit() == PropertyValue::Unit::PIXEL) {
                    heights[i] = (float)elements[i].rowHeight;// - ImGui::GetStyle().ItemSpacing.y;
                    absSum += (float)elements[i].rowHeight;
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
                if (elements[i].rowHeight.getUnit() == PropertyValue::Unit::PERCENT) {
                    float proportion = std::clamp((float)elements[i].rowHeight / 100.f, 0.f, 1.f);
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
                if (elements[i].rowHeight.getUnit() == PropertyValue::Unit::UNITLESS) {
                    ulSum += (float)elements[i].rowHeight;
                }
            }
            for (size_t i = 0; i < num; i++) {
                if (elements[i].rowHeight.getUnit() == PropertyValue::Unit::UNITLESS) {
                    if (ulSum != 0) {
                        float proportion = (float)elements[i].rowHeight / ulSum;
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