#pragma once

#include "BatteryUI/common.h"

namespace BatteryUI {

	// A 'Property' defines one value of a UI element. Parses to normal values.
	// An 'BasicImGuiProperty' is a different class with value which parses to ImGui values
	// Derived from it is 'ImGuiBridgeProperty', which holds ImGui values but parses json to simple values.

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
		explicit Property(T value) {
			this->value = value;
			_has_value = true;
		}

		Property& operator=(const T& value) {
			this->value = value;
			_has_value = true;
            return *this;
		}

		void get(T& v) {
			if (has_value()) {
				v = value;
			}
		}

		[[nodiscard]] bool has_value() const {
			return _has_value;
		}

	private:
		T value = T();
		bool _has_value = false;

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
	};
	








	
	
	class BasicImGuiProperty {
	public:
		
		BasicImGuiProperty() = default;
		BasicImGuiProperty(ImGuiStyleVar_ var, float value) {	// Loading an ImGuiStyleVar Variable as float
			type = PropertyType::STYLEVAR_FLOAT;
			imguiEnum = (int)var;
			this->value = value;
		}
        BasicImGuiProperty(ImGuiStyleVar_ var, ImVec2 vec2) {	// Loading an ImGuiStyleVar Variable as vec2
            type = PropertyType::STYLEVAR_VEC2;
            imguiEnum = (int)var;
            this->vec2 = vec2;
        }
		BasicImGuiProperty(ImGuiCol_ var, ImVec4 color) {		// Loading an ImGuiColor Variable as ImVec4
			type = PropertyType::COLOR;
			imguiEnum = (int)var;
			this->color = color;
		}
		BasicImGuiProperty(PropertyType type, float value) {		// Loading an ImGuiStyleVar Variable as float
			if (type != PropertyType::ITEM_WIDTH)
				throw UI_EXCEPTION("ITEM_WIDTH constructor cannot be called with a non-ITEM_WIDTH enum!");
			
			this->type = type;
			this->value = value;
		}
		
		void push() {
			switch (type) {
                case PropertyType::STYLEVAR_FLOAT:
                    ImGui::PushStyleVar(static_cast<ImGuiStyleVar_>(imguiEnum), value);
                    break;
                case PropertyType::STYLEVAR_VEC2:
                    ImGui::PushStyleVar(static_cast<ImGuiStyleVar_>(imguiEnum), vec2);
                    break;
                case PropertyType::COLOR:
                    ImGui::PushStyleColor(static_cast<ImGuiCol_>(imguiEnum), color);
                    break;
                case PropertyType::ITEM_WIDTH:
                    ImGui::PushItemWidth(value);
                    break;
			    default: break;
			}
		}

		void pop() {
			switch (type) {
                case PropertyType::STYLEVAR_FLOAT:
                case PropertyType::STYLEVAR_VEC2:
                case PropertyType::COLOR:
                case PropertyType::ITEM_WIDTH:
                    ImGui::PopItemWidth();
                    break;
                default: break;
			}
		}

        virtual BasicImGuiProperty& operator=(float v) {
			switch (type) {
                case PropertyType::STYLEVAR_FLOAT: value = v; break;
                case PropertyType::STYLEVAR_VEC2:
                    throw UI_EXCEPTION("Cannot set VEC2 property to a value of type float"); break;
                case PropertyType::COLOR:
                    throw UI_EXCEPTION("Cannot set color property to a value of type float"); break;
                case PropertyType::ITEM_WIDTH: value = v; break;
                default: throw UI_EXCEPTION("Invalid property type");
			}
            return *this;
		}

        virtual BasicImGuiProperty& operator=(ImVec2 v) {
            switch (type) {
                case PropertyType::STYLEVAR_FLOAT:
                    throw UI_EXCEPTION("Cannot set float property to a value of type ImVec2"); break;
                case PropertyType::STYLEVAR_VEC2: vec2 = v; break;
                case PropertyType::COLOR:
                    throw UI_EXCEPTION("Cannot set Color property to a value of type ImVec2"); break;
                case PropertyType::ITEM_WIDTH:
                    throw UI_EXCEPTION("Cannot set ITEM_WIDTH property to a value of type ImVec2"); break;
                default: throw UI_EXCEPTION("Invalid property type");
            }
            return *this;
        }

        virtual BasicImGuiProperty& operator=(ImVec4 c) {
			switch (type) {
                case PropertyType::STYLEVAR_FLOAT:
                    throw UI_EXCEPTION("Cannot set float property to a value of type ImVec4"); break;
                case PropertyType::STYLEVAR_VEC2:
                    throw UI_EXCEPTION("Cannot set VEC2 property to a value of type ImVec4"); break;
                case PropertyType::COLOR: color = c; break;
                case PropertyType::ITEM_WIDTH:
                    throw UI_EXCEPTION("Cannot set ITEM_WIDTH property to a value of type ImVec4"); break;
                default: throw UI_EXCEPTION("Invalid property type");
			}
            return *this;
		}

		virtual explicit operator float() {
			switch (type) {
                case PropertyType::STYLEVAR_FLOAT: return value;
                case PropertyType::STYLEVAR_VEC2:
                    throw UI_EXCEPTION("Cannot retrieve float value from a property of type VEC2"); break;
                case PropertyType::COLOR:
                    throw UI_EXCEPTION("Cannot retrieve float value from a property of type COLOR"); break;
                case PropertyType::ITEM_WIDTH: return value;
                default: throw UI_EXCEPTION("Invalid property type");
			}
		}

        virtual explicit operator ImVec2() {
            switch (type) {
                case PropertyType::STYLEVAR_FLOAT:
                    throw UI_EXCEPTION("Cannot retrieve ImVec2 value from a property of type float");
                case PropertyType::STYLEVAR_VEC2:
                    return vec2;
                case PropertyType::COLOR:
                    throw UI_EXCEPTION("Cannot retrieve ImVec2 value from a property of type ImVec4");
                case PropertyType::ITEM_WIDTH:
                    throw UI_EXCEPTION("Cannot retrieve ImVec2 value from a property of type ITEM_WIDTH");
                default: throw UI_EXCEPTION("Invalid property type");
            }
        }

		virtual explicit operator ImVec4() {
			switch (type) {
                case PropertyType::STYLEVAR_FLOAT:
                    throw UI_EXCEPTION("Cannot retrieve ImVec4 value from a property of type float");
                case PropertyType::STYLEVAR_VEC2:
                    throw UI_EXCEPTION("Cannot retrieve ImVec4 value from a property of type VEC2");
                case PropertyType::COLOR:
                    return color;
                case PropertyType::ITEM_WIDTH:
                    throw UI_EXCEPTION("Cannot retrieve ImVec4 value from a property of type ITEM_WIDTH");
                default: throw UI_EXCEPTION("Invalid property type");
			}
		}

	private:
		template<typename T>
		static std::string enumToString(int _enum) {
			auto opt = magic_enum::enum_cast<T>(_enum);
            return opt.has_value() ? std::string(magic_enum::enum_name(opt.value())) : "";
		}

		static PropertyType stringToTypeEnum(const std::string& _enum) {
			auto opt = magic_enum::enum_cast<PropertyType>(_enum);
			return opt.has_value() ? opt.value() : PropertyType::NONE;
		}

		template<typename T>
		static T stringToEnum(const std::string& _enum) {
			auto opt = magic_enum::enum_cast<T>(_enum);
			return opt.has_value() ? opt.value() : (T)0;
		}

		// JSON serialization
		/*friend void to_json(nlohmann::json& j, const BasicImGuiProperty& p) {
			switch (p.type) {
                case PropertyType::STYLEVAR_FLOAT:
                    j = nlohmann::json{
                        { "enum",  BasicImGuiProperty::enumToString<ImGuiStyleVar_>(p.imguiEnum)},
                        { "type",  magic_enum::enum_name(p.type) },
                        { "value", p.value }
                    }; break;

                case PropertyType::STYLEVAR_VEC2:
                    j = nlohmann::json{
                            { "enum",  BasicImGuiProperty::enumToString<ImGuiStyleVar_>(p.imguiEnum)},
                            { "type",  magic_enum::enum_name(p.type) },
                            { "value", p.vec2 }
                    }; break;

                case PropertyType::COLOR:
                    j = nlohmann::json{
                        { "enum",  BasicImGuiProperty::enumToString<ImGuiCol_>(p.imguiEnum)},
                        { "type",  magic_enum::enum_name(p.type) },
                        { "color", p.color }
                    }; break;

                case PropertyType::ITEM_WIDTH:
                    j = nlohmann::json{
                        { "type", magic_enum::enum_name(p.type) },
                        { "value", p.value }
                    }; break;

                default:    // No value
                    j = nullptr;
                    break;
            }
		}

		friend void from_json(const nlohmann::json& j, BasicImGuiProperty& p) {
			p.imguiEnum = 0;
			p.value = 0;
			p.color = ImVec4(0, 0, 0, 0);

			try {
				p.type = BasicImGuiProperty::stringToEnum<PropertyType>(j.at("type"));

				switch (p.type) {
                    case PropertyType::STYLEVAR_FLOAT:
                        p.imguiEnum = (int)BasicImGuiProperty::stringToEnum<ImGuiStyleVar_>(j.at("enum"));
                        p.value = j.at("value");
                        break;

                    case PropertyType::STYLEVAR_VEC2:
                        p.imguiEnum = (int)BasicImGuiProperty::stringToEnum<ImGuiStyleVar_>(j.at("enum"));
                        p.vec2 = j.at("value");
                        break;

                    case PropertyType::COLOR:
                        p.imguiEnum = (int)BasicImGuiProperty::stringToEnum<ImGuiStyleVar_>(j.at("enum"));
                        p.color = j.at("color");
                        break;

                    case PropertyType::ITEM_WIDTH:
                        p.value = j.at("value");
                        break;

                    default: break;
				}
			}
			catch (...) {
				p.type = PropertyType::NONE;
			}
		}*/

    protected:
        PropertyType type = PropertyType::NONE;
        float value = 0;
        int imguiEnum = 0;
        ImVec2 vec2 = ImVec2(0, 0);
        ImVec4 color = ImVec4(0, 0, 0, 0);
    };
	








	// This class can be pushed as an ImGui property but parses to a simple value
	/*class ImGuiBridgeProperty : public BasicImGuiProperty {
	public:
        ImGuiBridgeProperty() = default;
        ImGuiBridgeProperty(ImGuiStyleVar_ var, float value) : _property(var, value) {}
        ImGuiBridgeProperty(ImGuiStyleVar_ var, ImVec2 vec2) : _property(var, vec2) {}
        ImGuiBridgeProperty(ImGuiCol_ var, ImVec4 color) : _property(var, color) {}
        ImGuiBridgeProperty(PropertyType type, float value) : _property(type, value) {}

	protected:
        BasicImGuiProperty _property;

        // JSON serialization
        inline void to_json(nlohmann::json& j, const ImGuiBridgeProperty& p) {
            if (p.type == PropertyType::STYLEVAR_FLOAT) {
                j = nlohmann::json{ p.value };
            }
            if (p.type == PropertyType::STYLEVAR_VEC2) {
                j = nlohmann::json{ p.vec2 };
            }
            else if (p.type == PropertyType::COLOR) {
                j = nlohmann::json{ p.color };
            }
            else if (p.type == PropertyType::ITEM_WIDTH) {
                j = nlohmann::json{ p.value };
            }
            else {
                j = nlohmann::json{ nullptr };	// NULL
            }
        }

        inline void from_json(const nlohmann::json& j, ImGuiBridgeProperty& p) {
            if (p.type == PropertyType::STYLEVAR_FLOAT) {
                j.get_to(p.value);
            }
            if (p.type == PropertyType::STYLEVAR_VEC2) {
                j.get_to(p.vec2);
            }
            else if (p.type == PropertyType::COLOR) {
                j.get_to(p.color);
            }
            else if (p.type == PropertyType::ITEM_WIDTH) {
                j.get_to(p.value);
            }
            else {
                UI_EXCEPTION("Cannot load ImGuiBridgeProperty from json: Property has no defined type!");
            }
        }
	};*/

    // This class is an ImGui Property, but parses directly to ImVec2
    class ImGuiVec2 : BasicImGuiProperty {
    public:
        ImGuiVec2() {
            type = PropertyType::STYLEVAR_VEC2;
        };
        ImGuiVec2(ImGuiStyleVar_ _enum, const ImVec2& value) : BasicImGuiProperty(_enum, value) {}

        ImGuiVec2& operator=(const ImVec2& c) {
            switch (type) {
                case PropertyType::STYLEVAR_FLOAT:
                    throw UI_EXCEPTION("Cannot set float property to a value of type ImVec4"); break;
                case PropertyType::STYLEVAR_VEC2:
                    throw UI_EXCEPTION("Cannot set VEC2 property to a value of type ImVec4"); break;
                case PropertyType::COLOR: color = c; break;
                case PropertyType::ITEM_WIDTH:
                    throw UI_EXCEPTION("Cannot set ITEM_WIDTH property to a value of type ImVec4"); break;
                default: throw UI_EXCEPTION("Invalid property type");
            }
            return *this;
        }

        explicit operator ImVec2() {
            return vec2;
        }

        // JSON serialization
        inline void to_json(nlohmann::json& j, const ImGuiVec2& p) {
            j = nlohmann::json{ p.vec2 };
        }

        inline void from_json(const nlohmann::json& j, ImGuiVec2& p) {
            j.get_to(p.vec2);
        }
    };
	
}
