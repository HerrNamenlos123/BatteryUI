#pragma once

#include "BatteryUI/common.h"

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

        virtual void get(T& v) {
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

        void get(float& v) override {
            Property::get(v);
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

        void get(ImVec2& v) override {
            Property::get(v);
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

        void get(ImVec4& v) override {
            Property::get(v);
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
	
}
