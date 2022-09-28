#pragma once

#include <exception>

#ifdef _MSC_VER     // MSVC
#define GET_FUNCTION_NAME() __FUNCTION__    // Evaluates to 'myNamespace::myFunction'
#else
#define GET_FUNCTION_NAME() __func__        // Evaluates to 'myFunction'
#endif

#define UI_EXCEPTION(msg, ...) MakeException(GET_FUNCTION_NAME(), msg, ##__VA_ARGS__)

namespace BatteryUI {

	template<typename... TArgs>
    std::string _format_string_(const std::string& fmt, TArgs... args) {
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
        explicit Exception(const std::string& msg) {
            this->msg = msg;
        }

		[[nodiscard]] const char* what() const noexcept override {
			return msg.c_str();
		}

    private:
        std::string msg;
    };

    template<typename... TArgs>
    BatteryUI::Exception MakeException(const std::string& function, const std::string& fmt, TArgs... args) {
        return BatteryUI::Exception(_format_string_("[%s]: " + fmt, function.c_str(), args...));
    }

}
