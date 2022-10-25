#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/Widgets/BasicWidget.h"
#include "BatteryUI/Widgets/Container.h"

namespace BatteryUI {

    class HorizontalGrid : public BasicWidget {
    public:
        struct Widget {
            BatteryUI::Callback callback;
            PropertyValue columnWidth;
        };

        std::vector<Widget> elements;
        WidgetStyle style;
        float height = -1;
        bool sameline = false;

        HorizontalGrid() : BasicWidget("HorizontalGrid") {}
        HorizontalGrid(const std::string& name) : BasicWidget(name) {}
        HorizontalGrid(std::initializer_list<BatteryUI::Callback> elements) : BasicWidget("HorizontalGrid") {
            for (auto& element : elements) {
                this->elements.emplace_back(Widget{element, PropertyValue("1") });
            }
        }
        HorizontalGrid(std::initializer_list<std::pair<BatteryUI::Callback, std::string>> elements) : BasicWidget("HorizontalGrid") {
            for (auto& element : elements) {
                this->elements.emplace_back(Widget{
                        element.first, PropertyValue(element.second)
                });
            }
        }
        HorizontalGrid(const std::string& name, std::initializer_list<BatteryUI::Callback> elements) : BasicWidget(name) {
            for (auto& element : elements) {
                this->elements.emplace_back(Widget{element, PropertyValue("1") });
            }
        }
        HorizontalGrid(const std::string& name, std::initializer_list<std::pair<BatteryUI::Callback, std::string>> elements)
         : BasicWidget(name) {
            for (auto& element : elements) {
                this->elements.emplace_back(Widget{
                        element.first, PropertyValue(element.second)
                });
            }
        }

        void operator()(const BatteryUI::Callback& callback = nullptr) override {
            calculateWidths();
            ImGui::PushID(getIdentifier().c_str());
            //ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

            if (sameline)
                ImGui::SameLine();

            child.sameline = false;
            for (size_t i = 0; i < elements.size(); i++) {

                child.style["ContainerBorderColor"] = RetrieveProperty("GridBorderColor", ImVec4(255, 255, 255, 255) / 255);
                child.style["ContainerBorderWidth"] = RetrieveProperty("GridBorderWidth", 0.f);

                child.size = { widths[i], height };

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

        BATTERYUI_SERIALIZE(HorizontalGrid, name, sameline);

    private:
        void calculateWidths() {
            size_t num = elements.size();
            widths.resize(num);
            std::fill(widths.begin(), widths.end(), 0.f);

            // First apply all absolute values
            float absSum = 0;
            for (size_t i = 0; i < num; i++) {
                if (elements[i].columnWidth.getUnit() == PropertyValue::Unit::PIXEL) {
                    widths[i] = (float)elements[i].columnWidth;// - ImGui::GetStyle().ItemSpacing.x;
                    absSum += (float)elements[i].columnWidth;
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
                if (elements[i].columnWidth.getUnit() == PropertyValue::Unit::PERCENT) {
                    float proportion = std::clamp((float)elements[i].columnWidth / 100.f, 0.f, 1.f);
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
                if (elements[i].columnWidth.getUnit() == PropertyValue::Unit::UNITLESS) {
                    ulSum += (float)elements[i].columnWidth;
                }
            }
            for (size_t i = 0; i < num; i++) {
                if (elements[i].columnWidth.getUnit() == PropertyValue::Unit::UNITLESS) {
                    if (ulSum != 0) {
                        float proportion = (float)elements[i].columnWidth / ulSum;
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