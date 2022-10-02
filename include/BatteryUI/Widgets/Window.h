#pragma once

#include <string>
#include <functional>

#include "BatteryUI/common.h"
#include "BatteryUI/Style.h"
#include "BatteryUI/Widgets/BasicWidget.h"

namespace BatteryUI {
	
	class Window : public BasicWidget {
	public:
		Window() : BasicWidget("BatteryUI Window") {}
		explicit Window(const std::string& name, int flags = 0) : BasicWidget(name), flags(flags) {}

		void operator()(const std::function<void(void)>& callback) {
			draw(callback);
		}

	private:
		void draw(const std::function<void(void)>& callback);

		int flags = 0;
	};

	inline void Window::draw(const std::function<void(void)>& callback) {
		ImGui::Begin(getIdentifier().c_str(), nullptr, flags);

		callback();		// Do the user rendering here

		ImGui::End();
	}
	
}