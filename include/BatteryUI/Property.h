#pragma once

#include "BatteryUI/common.h"

namespace BatteryUI {

	// A 'Property' defines one value of a UI element. Parses to normal values.
	// An 'ImGuiProperty' is a different class with value which parses to ImGui values
	// Derived from it is 'ImGuiBridgeProperty', which holds ImGui values but parses to normal text.

	enum class PropertyType {
		NONE = 0,
		STYLEVAR,
		COLOR,
		ITEM_WIDTH,
	};

	template<typename T>
	class Property {
	public:

		Property() {}
		Property(T value) {
			this->value = value;
			_has_value = true;
		}

		void operator=(const T& value) {
			this->value = value;
			_has_value = true;
		}

		void get(T& v) {
			if (has_value()) {
				v = value;
			}
		}

		bool has_value() const {
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
	








	
	
	class ImGuiProperty {
	public:
		
		ImGuiProperty() {}
		ImGuiProperty(ImGuiStyleVar_ var, float value) {	// Loading an ImGuiStyleVar Variable as float
			type = PropertyType::STYLEVAR;
			imguiEnum = (int)var;
			this->value = value;
			this->color = { 0, 0, 0, 0 };
		}
		ImGuiProperty(ImGuiCol_ var, ImVec4 color) {		// Loading an ImGuiColor Variable as ImVec4
			type = PropertyType::COLOR;
			imguiEnum = (int)var;
			this->value = 0.f;
			this->color = color;
		}
		ImGuiProperty(PropertyType type, float value) {		// Loading an ImGuiStyleVar Variable as float
			if (type != PropertyType::ITEM_WIDTH)
				throw UI_EXCEPTION("ITEM_WIDTH constructor was called with a non-ITEM_WIDTH enum!");
			
			this->type = type;
			imguiEnum = 0;
			this->value = value;
			this->color = { 0, 0, 0, 0 };
		}
		
		void push() {
			switch (type) {
			case PropertyType::STYLEVAR: 
				ImGui::PushStyleVar(static_cast<ImGuiStyleVar_>(imguiEnum), value); 
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
			case PropertyType::STYLEVAR:
				ImGui::PopStyleVar();
				break;
			case PropertyType::COLOR:
				ImGui::PopStyleColor();
				break;
			case PropertyType::ITEM_WIDTH:
				ImGui::PopItemWidth();
				break;
			default: break;
			}
		}

		void operator=(float v) {
			switch (type) {
			case PropertyType::STYLEVAR: value = v; break;
			case PropertyType::COLOR:
				throw UI_EXCEPTION("Cannot set color property to a value of type float"); break;
			case PropertyType::ITEM_WIDTH: value = v; break;
			default: throw UI_EXCEPTION("Invalid property type");
			}
		}

		void operator=(ImVec4 c) {
			switch (type) {
			case PropertyType::STYLEVAR:
				throw UI_EXCEPTION("Cannot set STYLEVAR property to a value of type ImVec4"); break;
			case PropertyType::COLOR: color = c; break;
			case PropertyType::ITEM_WIDTH:
				throw UI_EXCEPTION("Cannot set ITEM_WIDTH property to a value of type ImVec4"); break;
			default: throw UI_EXCEPTION("Invalid property type");
			}
		}

		operator float() {
			switch (type) {
			case PropertyType::STYLEVAR: return value;
			case PropertyType::COLOR:
				throw UI_EXCEPTION("Cannot retrieve float value from a property of type COLOR"); break;
			case PropertyType::ITEM_WIDTH: return value;
			default: throw UI_EXCEPTION("Invalid property type");
			}
		}

		operator ImVec4() {
			switch (type) {
			case PropertyType::STYLEVAR:
				throw UI_EXCEPTION("Cannot retrieve ImVec4 value from a property of type STYLEVAR");
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
			if (opt.has_value()) {
				return std::string(magic_enum::enum_name(opt.value()));
			}
			else {
				return "";
			}
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

	private:
		PropertyType type = PropertyType::NONE;

		int imguiEnum = 0;
		float value = 0;
		ImVec4 color = ImVec4(0, 0, 0, 0);

		// JSON serialization
		friend void to_json(nlohmann::json& j, const ImGuiProperty& p) {
			std::string _enum;
			switch (p.type) {
			case PropertyType::STYLEVAR:
				j = nlohmann::json{
					{ "enum", ImGuiProperty::enumToString<ImGuiStyleVar_>(p.imguiEnum)},
					{ "type", magic_enum::enum_name(p.type) },
					{ "value", p.value }
				}; break;

			case PropertyType::COLOR:
				j = nlohmann::json{
					{ "enum", ImGuiProperty::enumToString<ImGuiCol_>(p.imguiEnum)},
					{ "type", magic_enum::enum_name(p.type) },
					{ "color", p.color }
				}; break;

			case PropertyType::ITEM_WIDTH:
				j = nlohmann::json{
					{ "type", magic_enum::enum_name(p.type) },
					{ "value", p.value }
				}; break;

			default: break;
			}
		}

		friend void from_json(const nlohmann::json& j, ImGuiProperty& p) {
			p.imguiEnum = 0;
			p.value = 0;
			p.color = ImVec4(0, 0, 0, 0);

			try {
				p.type = ImGuiProperty::stringToEnum<PropertyType>(j.at("type"));

				switch (p.type) {
				case PropertyType::STYLEVAR:
					p.imguiEnum = (int)ImGuiProperty::stringToEnum<ImGuiStyleVar_>(j.at("enum"));
					p.value = j.at("value");
					break;

				case PropertyType::COLOR:
					p.imguiEnum = (int)ImGuiProperty::stringToEnum<ImGuiStyleVar_>(j.at("enum"));
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
		}
	};
	








	/*
	class HiddenImGuiProperty : public ImGuiProperty {
	public:
		HiddenImGuiProperty() {}
		HiddenImGuiProperty(ImGuiStyleVar_ var, float value) : _property(var, value) {}
		HiddenImGuiProperty(ImGuiCol_ var, ImVec4 color) : _property(var, color) {}
		HiddenImGuiProperty(PropertyType type, float value) : _property(type, value) {}

	private:
		ImGuiProperty _property;

		friend void to_json(nlohmann::json& j, const HiddenImGuiProperty& p);
		friend void from_json(const nlohmann::json& j, HiddenImGuiProperty& p);
	};

	// JSON serialization
	inline void to_json(nlohmann::json& j, const HiddenImGuiProperty& p) {
		if (p.type == PropertyType::STYLEVAR) {
			j = nlohmann::json{ p.value };
		}
		else if (p.type == PropertyType::COLOR) {
			j = nlohmann::json{ p.color };
		}
		else if (p.type == PropertyType::ITEM_WIDTH) {
			j = nlohmann::json{ p.value };
		}
		else {
			j = nlohmann::json{ 0 };	// NULL
		}
	}

	inline void from_json(const nlohmann::json& j, HiddenImGuiProperty& p) {
		if (p.type == PropertyType::STYLEVAR) {
			j.get_to(p.value);
		}
		else if (p.type == PropertyType::COLOR) {
			j.get_to(p.color);
		}
		else if (p.type == PropertyType::ITEM_WIDTH) {
			j.get_to(p.value);
		}
	}*/
	
}
