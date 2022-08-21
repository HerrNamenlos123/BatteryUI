#pragma once

#include <exception>

#ifndef _MSC_VER
#define __FUNCTION__ __func__
#endif

#define UI_EXCEPTION(msg, ...) MakeException(__FUNCTION__, msg, ##__VA_ARGS__)

namespace BatteryUI {

	template<typename... TArgs>
    std::string __format_string_(const std::string& fmt, TArgs... args) {
        int size = 100;
        std::string str;
        while (true) {
            str.resize(size);
            int n = std::snprintf(&str[0], size, fmt.c_str(), args...);
            if (n > -1 && n < size) {
                str.resize(n); // Make sure there are no trailing zero char
                return str;
            }
            if (n > -1) size = n + 1;
            else size *= 2;
        }
    }

    class Exception : public std::exception {
    public:
        Exception(const std::string& msg) {
            this->msg = msg;
        }

		const char* what() const noexcept override {
			return msg.c_str();
		}

    private:
        std::string msg;
    };

    template<typename... TArgs>
    BatteryUI::Exception MakeException(const std::string& function, const std::string& fmt, TArgs... args) {
        return BatteryUI::Exception(__format_string_("[%s]: " + fmt, function.c_str(), args...));
    }

}
