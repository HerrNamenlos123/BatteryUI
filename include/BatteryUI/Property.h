#pragma once

#include "BatteryUI/common.h"

namespace BatteryUI {

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

		friend void to_json(nlohmann::json& j, const Property& p);
		friend void from_json(const nlohmann::json& j, Property& p);
	};

	// JSON serialization
	template<typename T>
	inline void to_json(nlohmann::json& j, const Property<T>& p) {
		if (p._has_value) {
			j = p.value;
		}
	}

	template<typename T>
	inline void from_json(const nlohmann::json& j, Property<T>& p) {
		p._has_value = false;
		try {
			j.get_to(p.value);
			p._has_value = true;
		}
		catch (...) {}
	}
	








	
	
	class ImGuiProperty {
	public:
		
		ImGuiProperty() {}
		ImGuiProperty(ImGuiStyleVar_ var, float value) {
			imguiEnum = (int)var;
			type = PropertyType::STYLEVAR;
			this->value = value;
		}
		ImGuiProperty(ImGuiCol_ var, ImVec4 color) {
			imguiEnum = (int)var;
			type = PropertyType::COLOR;
			this->color = color;
		}
		ImGuiProperty(PropertyType type, float value) {
			this->type = type;
			this->value = value;
		}
		
		void push() {
			if (type == PropertyType::STYLEVAR) {
				ImGui::PushStyleVar(static_cast<ImGuiStyleVar_>(imguiEnum), value);
			}
			else if (type == PropertyType::COLOR) {
				ImGui::PushStyleColor(static_cast<ImGuiCol_>(imguiEnum), color);
			}
			else if (type == PropertyType::ITEM_WIDTH) {
				ImGui::PushItemWidth(value);
			}
		}

		void pop() {
			if (type == PropertyType::STYLEVAR) {
				ImGui::PopStyleVar();
			}
			else if (type == PropertyType::COLOR) {
				ImGui::PopStyleColor();
			}
			else if (type == PropertyType::ITEM_WIDTH) {
				ImGui::PopItemWidth();
			}
		}

		void operator=(float v) {
			if (type == PropertyType::STYLEVAR) {
				value = v;
			}
			else if (type == PropertyType::COLOR) {
				throw MakeException(__FUNCTION__, "Cannot set color property to a value of type float");
			}
			else if (type == PropertyType::ITEM_WIDTH) {
				value = v;
			}
		}

		void operator=(ImVec4 c) {
			if (type == PropertyType::STYLEVAR) {
				throw MakeException(__FUNCTION__, "Cannot set float property to a value of type ImVec4");
			}
			else if (type == PropertyType::COLOR) {
				color = c;
			}
			else if (type == PropertyType::ITEM_WIDTH) {
				throw MakeException(__FUNCTION__, "Cannot set float property to a value of type ImVec4");
			}
		}

		operator float() {
			if (type == PropertyType::STYLEVAR) {
				return value;
			}
			else if (type == PropertyType::COLOR) {
				throw MakeException(__FUNCTION__, "Cannot retrieve float value from a property of type color");
			}
			else if (type == PropertyType::ITEM_WIDTH) {
				return value;
			}
			else {
				throw MakeException(__FUNCTION__, "Invalid property type");
			}
		}

		operator ImVec4() {
			if (type == PropertyType::STYLEVAR) {
				throw MakeException(__FUNCTION__, "Cannot retrieve ImVec4 value from a property of type float");
			}
			else if (type == PropertyType::COLOR) {
				return color;
			}
			else if (type == PropertyType::ITEM_WIDTH) {
				throw MakeException(__FUNCTION__, "Cannot retrieve ImVec4 value from a property of type float");
			}
			else {
				throw MakeException(__FUNCTION__, "Invalid property type");
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

	protected:
		int imguiEnum = 0;
		PropertyType type = PropertyType::NONE;

		float value = 0;
		ImVec4 color = ImVec4(0, 0, 0, 0);

		friend void to_json(nlohmann::json& j, const ImGuiProperty& p);
		friend void from_json(const nlohmann::json& j, ImGuiProperty& p);
	};

	// JSON serialization
	inline void to_json(nlohmann::json& j, const ImGuiProperty& p) {
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

	inline void from_json(const nlohmann::json& j, ImGuiProperty& p) {
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
		else {
			j = nlohmann::json{ 0 };	// NULL
		}
	}
	
}
