#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/Style.h"
#include "BatteryUI/Widgets/BasicWidget.h"

namespace BatteryUI {
	
	class Window : public BasicWidget {
	public:

        int flags = ImGuiWindowFlags_None;

		Window() : BasicWidget("BatteryUI Window") {}
		explicit Window(const std::string& name, int flags = 0) : BasicWidget(name), flags(flags) {}

		void operator()(const BatteryUI::Callback& callback) {
            ImGui::Begin(getIdentifier().c_str(), nullptr, flags);

            callback();		// Do the user rendering here

            ImGui::End();
		}

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Window, flags);
	};
	
}