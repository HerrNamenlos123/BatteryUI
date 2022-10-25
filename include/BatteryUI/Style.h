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

    class PropertyValue {
    public:

        enum class Unit {
            NONE,               // Invalid
            UNITLESS,           // eg. '2.7'                        -> (string)
            PIXEL,              // eg. '35.2px'                     -> (string)
            PERCENT,            // eg. '17%'                        -> (string)
            VEC2,               // eg. [45, 28.7] or ImVec2(3, 4)   -> (json/object)
            COLOR_HEX           // eg. '#FF0000' or '#FF0000FF'     -> (string)
        };

        PropertyValue() = default;
        PropertyValue(const char* value) {
            this->operator=(value);
        }
        PropertyValue(const std::string& value) {
            this->operator=(value);
        }
        PropertyValue(float value, Unit unit) {
            this->value_ = value;
            this->unit_ = unit;
        }
        PropertyValue(float value) {
            this->value_ = value;
            this->unit_ = Unit::UNITLESS;
        }
        PropertyValue(ImVec2 value) {
            this->operator=(value);
        }
        PropertyValue(ImVec4 color) {
            this->operator=(color);
        }

        PropertyValue& operator=(const char* value) {
            this->operator=(std::string(value));
            return *this;
        }

        PropertyValue& operator=(const std::string& value) {
            if (value.empty())                                              // Is empty
                throw UI_EXCEPTION("Cannot load value: Value string is empty!");

            if (value[0] == '#') {
                parseColor(value);
            }
            else {
                parseUnitlessPixelPercent(value);
            }

            return *this;
        }

        PropertyValue& operator=(float value) {
            this->value_ = value;
            this->unit_ = Unit::UNITLESS;
            return *this;
        }

        PropertyValue& operator=(ImVec2 value) {
            this->vec4 = ImVec4(value.x, value.y, 0, 0);
            unit_ = Unit::VEC2;
            return *this;
        }

        PropertyValue& operator=(ImVec4 color) {
            this->vec4 = color;
            unit_ = Unit::COLOR_HEX;
            return *this;
        }

        explicit operator float() const {
            return value_;
        }

        explicit operator ImVec2() const {
            return {vec4.x, vec4.y};
        }

        explicit operator ImVec4() const {
            return vec4;
        }

        [[nodiscard]] Unit getUnit() const {
            return unit_;
        }

        bool operator==(const PropertyValue& other) {
            return (this->unit_ == other.unit_) && (this->value_ == other.value_) && (this->vec4 == other.vec4);
        }

        bool operator!=(const PropertyValue& other) {
            return !this->operator==(other);
        }

        [[nodiscard]] std::string str() const {
            std::stringstream str;
            switch (unit_) {
                case Unit::PERCENT: str << std::setprecision(6) << value_ << "%"; break;
                case Unit::PIXEL: str << std::setprecision(6) << value_ << "px"; break;
                case Unit::COLOR_HEX: str << "#" << std::setfill('0') << std::hex << std::uppercase <<
                                          std::setw(2) << (int)vec4.x <<
                                          std::setw(2) << (int)vec4.y <<
                                          std::setw(2) << (int)vec4.z;
                            if (vec4.w != 0) str << std::setw(2) << (int)vec4.w; break;
                default: break;
            }
            return str.str();
        }

        explicit operator std::string() const {
            return this->str();
        }

    private:
        void parseUnitlessPixelPercent(const std::string& str) {
            float tempValue = 0.f;
            Unit tempUnit = Unit::NONE;

            if (std::string("0123456789.-").find(str[0]) == std::string::npos)       // Doesn't start with a digit
                throw UI_EXCEPTION("Cannot load value '%s': Expected a digit!", str.c_str());

            size_t numberDigits = str.find_first_not_of("0123456789.-");        // Scan how many digits we have
            if (numberDigits == std::string::npos) numberDigits = str.length();            // It must consist only of digits

            try {
                tempValue = std::stof(str.substr(0, numberDigits));
            }
            catch (...) {
                throw UI_EXCEPTION("Cannot load value '%s': Parsing as float failed", str.c_str());
            }

            bool digitsOnly = numberDigits == str.length();
            if (digitsOnly) {
                tempUnit = Unit::UNITLESS;
            }
            else {
                std::string unit = str.substr(numberDigits, str.length() - numberDigits);
                if (unit == "px") tempUnit = Unit::PIXEL;
                else if (unit == "%") tempUnit = Unit::PERCENT;
                else throw UI_EXCEPTION("Cannot load value '%s': Unit is not known!", str.c_str());
            }

            this->value_ = tempValue;
            this->unit_ = tempUnit;
        }

        void parseColor(const std::string& str) {
            this->value_ = 0.f;

            if (str.length() == 7) {
                // Parse #RRGGBB format
                uint8_t _color[4];
                std::stringstream ss;
                ss << std::hex << str.substr(1);
                ss >> *(uint32_t*)_color;
                vec4 = ImVec4(_color[2], _color[1], _color[0], 255);
                unit_ = Unit::COLOR_HEX;
            }
            else if (str.length() == 9) {
                // Parse #RRGGBBAA format
                uint8_t _color[4];
                std::stringstream ss;
                ss << std::hex << str.substr(1);
                ss >> *(uint32_t*)_color;
                vec4 = ImVec4(_color[3], _color[2], _color[1], _color[0]);
                unit_ = Unit::COLOR_HEX;
            }
            else {
                throw UI_EXCEPTION("Cannot load value '%s' as an HTML color code: Unexpected length!", str.c_str());
            }
        }

        float value_ = 0.f;
        ImVec4 vec4 = ImVec4(0, 0, 0, 0);
        Unit unit_ = Unit::NONE;
    };

    inline std::ostream& operator<<(std::ostream& os, const PropertyValue& value) {
        os << value.str();
        return os;
    }

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
