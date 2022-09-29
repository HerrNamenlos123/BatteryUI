#pragma once

#include "BatteryUI/common.h"

#define EXPORT_ITEM(var) //ar(CEREAL_NVP(var));
#define EXPORT_ITEM_NESTED(obj, var) //ar(cereal::make_nvp(#var, obj.var));

#define PUSH_STYLE(var, stylevar) \
	if (var.has_value()) { \
		ImGui::PushStyleVar(stylevar, var.value()); \
	}

#define POP_STYLE(var) \
	if (var.has_value()) { \
		ImGui::PopStyleVar(); \
	}

#define SAVE_ITEM_IF_VALUE(var) if (var.has_value()) EXPORT_ITEM(var);
#define SAVE_ITEM_IF_VALUE_OPT(T, var) if (var.has_value()) { T v = var.value(); EXPORT_ITEM(v); }
#define LOAD_ITEM_IF_EXISTS(var) //\
	try { \
		decltype(var) _var; \
		ar(cereal::make_nvp(#var, _var)); \
		var = _var; \
	} \
	catch (...) { var = decltype(var)(); }

#define LOAD_ITEM_IF_EXISTS_OPT(T, var) //\
	try { \
		T v; \
		ar(cereal::make_nvp(#var, v)); \
		var = v; \
	} \
	catch (...) { var = std::nullopt; }

#define EXPORT_STYLE() void ar(){} void foo()//\
	void archive(cereal::JSONInputArchive& ar) { ar(cereal::make_nvp("root", *this)); } \
	void archive(cereal::JSONOutputArchive& ar) { ar(cereal::make_nvp("root", *this)); } \
	template<class Archive> \
	void serialize(Archive& ar) 

namespace BatteryUI {

	struct vec2 {
		float x = 0, y = 0;
		vec2() = default;
		vec2(const vec2& v) : x(v.x), y(v.y) {};
		vec2(int x, int y) : x(x), y(y) {}
		vec2(float x, float y) : x(x), y(y) {}
		explicit vec2(ImVec2 v) : x(v.x), y(v.y) {}
		explicit operator ImVec2() { return { x, y }; }
		void operator=(const vec2& v) { x = v.x; y = v.y; }

		template <class Archive>
		void serialize(Archive& ar) {
			EXPORT_ITEM(x);
			EXPORT_ITEM(y);
		}
	};

	struct vec2_opt {
		vec2_opt() {}
		vec2_opt(int x, int y) : v(x, y) { _has_value = true; }
		vec2_opt(float x, float y) : v(x, y) { _has_value = true; }
		vec2_opt(ImVec2 v) : v(v.x, v.y) { _has_value = true; }
		void operator=(const vec2_opt& v) { this->v = v.v; _has_value = v._has_value; }

		bool has_value() const {
			return _has_value;
		}

		vec2 value() {
			if (_has_value)
				return v;
			else
				return vec2();
		}

		template <class Archive>
		void save(Archive& ar) const {
			if (_has_value) {
				float x = v.x;
				float y = v.y;
				EXPORT_ITEM(x);
				EXPORT_ITEM(y);
			}
		}

		template <class Archive>
		void load(Archive& ar) {
			try {
				float x = 0, y = 0;
				EXPORT_ITEM(x);
				EXPORT_ITEM(y);
				v = vec2(x, y);
				_has_value = true;
			}
			catch (...) {
				v = vec2(0, 0);
				_has_value = false;
			}
		}

	private:
		vec2 v;
		bool _has_value = false;
	};
}
