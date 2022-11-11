#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/FileWatcher.h"
#include "BatteryUI/Property.h"
#include "BatteryUI/InternalDecl.h"

namespace BatteryUI {

	/*struct ColorScheme {

		struct Color {
			float r = 0, g = 0, b = 0, a = 0;
			Color() {}
			Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
			Color(ImVec4 c) : r(c.x), g(c.y), b(c.z), a(c.w) {}
			operator ImVec4() {
				return { r, g, b, a };
			}
		};

		struct ColorEntry {
			std::string identifier;
			Color color;
		};

		[[nodiscard]] size_t size() const {
			return colors.size();
		}

		void load() {
			colors.clear();
			for (size_t i = 0; i < ImGuiCol_COUNT; i++) {
				ColorScheme::ColorEntry entry;
				entry.identifier = magic_enum::enum_name((ImGuiCol_)i).data();
				entry.color = ImGui::GetStyle().Colors[i];
				colors.push_back(entry);
			}
		}

		void apply() {
			ImVec4* c = ImGui::GetStyle().Colors;

			for (auto& entry : colors) {
				size_t index = findImguiIndex(entry.identifier);
				if (index != -1)
					c[index] = entry.color;
			}
		}

		size_t findImguiIndex(const std::string& id) {
			for (size_t i = 0; i < ImGuiCol_COUNT; i++) {
				auto identifier = magic_enum::enum_name((ImGuiCol_)i);
				if (identifier == id) {
					return i;
				}
			}
			return -1;
		}

		void push() {
			for (auto& entry : colors) {
				size_t index = findImguiIndex(entry.identifier);
				if (index != -1)
					ImGui::PushStyleColor(index, entry.color);
			}
		}

		void pop() {
			for (auto& entry : colors) {
				size_t index = findImguiIndex(entry.identifier);
				if (index != -1)
					ImGui::PopStyleColor();
			}
		}

		void addColor(const std::string& identifier, float r, float g, float b, float a) {
			ColorScheme::ColorEntry entry;
			entry.identifier = identifier;
			entry.color = { r, g, b, a };
			colors.push_back(entry);
		}

		std::vector<ColorEntry> colors;
	};*/

    class WidgetStyle {
    public:
        WidgetStyle() = default;

        PropertyValue& operator[](const char* str) {
            return values[str];
        }

        PropertyValue& operator[](const std::string& str) {
            return this->operator[](str.c_str());
        }

        void push() {
            if (pushed)
                throw UI_EXCEPTION("Cannot push WidgetStyle property: Push/Pop mismatch!");

            pushed = true;
            for (auto& [property, value] : values) {
                BatteryUI::PushProperty(property, value);
            }
        }

        void pop() {
            if (!pushed)
                throw UI_EXCEPTION("Cannot push WidgetStyle property: Push/Pop mismatch!");

            pushed = false;
            for (auto& [property, value] : values) {
                BatteryUI::PopProperty();
            }
        }

    private:
        bool pushed = false;
        std::unordered_map<std::string, PropertyValue> values;
    };

}
