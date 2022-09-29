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
