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

#include "BatteryUI/json.hpp"
#include "BatteryUI/magic_enum.h"
#include "BatteryUI/std_function.h"

namespace BatteryUI {
    template<typename T>
    using Function = func::function<T>;     // This is a cheap replacement for std::function to reduce compile time
    using Callback = BatteryUI::Function<void(void)>;
}

#include "BatteryUI/exception.h"

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
