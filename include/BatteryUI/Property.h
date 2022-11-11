#pragma once

#include "BatteryUI/common.h"

// Default is the lowest priority, then be the global default, then the per-item value
#define UI_PROPERTY_PRIORITY(type, var, default_, prio1, prio2) \
    type var = default_;  \
    prio1.get_to(var); \
    prio2.get_to(var);

namespace BatteryUI {

	enum class PropertyType {
		NONE = 0,
		STYLEVAR_FLOAT,
        STYLEVAR_VEC2,
		COLOR,
		ITEM_WIDTH
	};

	template<typename T>
	class Property {
	public:

		Property() = default;
		Property(T value) {
			this->value = value;
			_has_value = true;
		}

        virtual Property& operator=(const T& _value) {
			this->value = _value;
			_has_value = true;
            return *this;
		}

        virtual void get_to(T& v) {
			if (has_value()) {
				v = value;
			}
		}

		[[nodiscard]] bool has_value() const {
			return _has_value;
		}

		// JSON serialization
		friend void to_json(nlohmann::json& j, const Property& p) {
			if (p._has_value) {
				j = p.value;
			}
			else {
				j = nullptr;
			}
		}
		
		friend void from_json(const nlohmann::json& j, Property& p) {
			p._has_value = false;
			try {
				j.get_to(p.value);
				p._has_value = true;
			}
			catch (...) {}
		}

    protected:
        T value = T();
        bool _has_value = false;
	};

    template<ImGuiStyleVar_ defaultType = ImGuiStyleVar_COUNT>
    class ImGuiPropFloat : public Property<float> {
    public:
        ImGuiPropFloat() = default;
        ImGuiPropFloat(ImGuiStyleVar_ enum_) : enum_(enum_) {}
        ImGuiPropFloat(ImGuiStyleVar_ enum_, float v) : enum_(enum_), Property<float>(v) {}

        ImGuiPropFloat& operator=(const float& v) override {
            Property::operator=(v);
            return *this;
        }

        void get_to(float& v) override {
            Property::get_to(v);
        }

        void push() {
            if (enum_ < ImGuiStyleVar_COUNT && has_value()) ImGui::PushStyleVar(enum_, value);
        }

        void pop() {
            if (enum_ < ImGuiStyleVar_COUNT && has_value()) ImGui::PopStyleVar();
        }

    private:
        ImGuiStyleVar_ enum_ = defaultType;
    };

    template<ImGuiStyleVar_ defaultType = ImGuiStyleVar_COUNT>
    class ImGuiPropVec2 : public Property<ImVec2> {
    public:
        ImGuiPropVec2() = default;
        ImGuiPropVec2(ImGuiStyleVar_ enum_) : enum_(enum_) {}
        ImGuiPropVec2(ImGuiStyleVar_ enum_, ImVec2 v) : enum_(enum_), Property<ImVec2>(v) {}

        ImGuiPropVec2& operator=(const ImVec2& v) override {
            Property::operator=(v);
            return *this;
        }

        void get_to(ImVec2& v) override {
            Property::get_to(v);
        }

        void push() {
            if (enum_ < ImGuiStyleVar_COUNT && has_value()) ImGui::PushStyleVar(enum_, value);
        }

        void pop() {
            if (enum_ < ImGuiStyleVar_COUNT && has_value()) ImGui::PopStyleVar();
        }

    private:
        ImGuiStyleVar_ enum_ = defaultType;
    };

    template<ImGuiCol_ defaultType = ImGuiCol_COUNT>
    class ImGuiPropVec4 : public Property<ImVec4> {
    public:
        ImGuiPropVec4() = default;
        ImGuiPropVec4(ImGuiCol_ enum_) : enum_(enum_) {}
        ImGuiPropVec4(ImGuiCol_ enum_, ImVec4 v) : enum_(enum_), Property<ImVec4>(v) {}

        ImGuiPropVec4& operator=(const ImVec4& v) override {
            Property::operator=(v);
            return *this;
        }

        void get_to(ImVec4& v) override {
            Property::get_to(v);
        }

        void push() {
            if (enum_ < ImGuiCol_COUNT && has_value()) ImGui::PushStyleColor(enum_, value);
        }

        void pop() {
            if (enum_ < ImGuiCol_COUNT && has_value()) ImGui::PopStyleColor();
        }

    private:
        ImGuiCol_ enum_ = defaultType;
    };

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
	
}
