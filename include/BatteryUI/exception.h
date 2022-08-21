#pragma once

#include <exception>
#include <format>

#ifndef _MSC_VER
#define __FUNCTION__ __func__
#endif

#define UI_EXCEPTION(msg, ...) MakeException(__FUNCTION__, msg, __VA_ARGS__)

namespace BatteryUI {

    template<typename... TArgs>
    std::exception MakeException(const std::string& function, std::string_view fmt, TArgs... args) {
        return std::runtime_error(
            std::format("[{}]: ", function) + 
            std::vformat(fmt, std::make_format_args(std::forward<TArgs>(args)...)));
    }

}
