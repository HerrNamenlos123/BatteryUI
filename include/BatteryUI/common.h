#pragma once

#include <string>
#include <vector>
#include <optional>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <atomic>
#include <map>
#include <unordered_map>

#include "BatteryUI/External/json.hpp"
#include "BatteryUI/External/magic_enum.h"
#include "BatteryUI/External/std_function.h"
#include "BatteryUI/exception.h"
#include "BatteryUI/utility.h"

namespace BatteryUI {
    template<typename T>
    using Function = func::function<T>;     // This is a cheap replacement for std::function to reduce compile time
    using Callback = BatteryUI::Function<void(void)>;
}

#define BATTERYUI_SERIALIZE NLOHMANN_DEFINE_TYPE_INTRUSIVE

#ifdef BATTERY_COMPILER_GCC
#define VA_ARGS(...) __VA_OPT__(,) __VA_ARGS__
#else
#define VA_ARGS(...) , ##__VA_ARGS__
#endif

// JSON serialization for ImVec2
inline void to_json(nlohmann::json& j, const ImVec2& v) {
    j = nlohmann::json::array({ v.x, v.y });
}

inline void from_json(const nlohmann::json& j, ImVec2& v) {
    v.x = j.at(0);
    v.y = j.at(1);
}

// JSON serialization for ImVec4
inline void to_json(nlohmann::json& j, const ImVec4& v) {
	j = nlohmann::json::array({ v.x, v.y, v.z, v.w });
}

inline void from_json(const nlohmann::json& j, ImVec4& v) {
	v.x = j.at(0);
	v.y = j.at(1);
	v.z = j.at(2);
	v.w = j.at(3);
}

// ImVec4 operators
inline bool operator==(const ImVec4& first, const ImVec4& second) {
    return first.x == second.x && first.y == second.y && first.z == second.z && first.w == second.w;
}

inline ImVec4 operator/(const ImVec4& first, float value) {
    return {first.x / value, first.y / value, first.z / value, first.w / value};
}
