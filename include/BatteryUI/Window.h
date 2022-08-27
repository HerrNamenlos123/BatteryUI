#pragma once

#include <string>
#include <functional>

#include "BatteryUI/common.h"
#include "BatteryUI/serialize.h"
#include "BatteryUI/Style.h"
#include "BatteryUI/BasicWidget.h"

namespace BatteryUI {
	
	class Window : public BasicWidget {
	public:
		Window() {
			this->name = "BatteryUI Window";
		}
		
		Window(const std::string& name, int flags = 0) {
			this->name = name;
			this->flags = flags;
		}

		void draw(std::function<void(void)> callback) {
			style.push();
			ImGui::Begin(getIdentifier().c_str(), nullptr, flags);

			callback();		// Do the user rendering here
			
			ImGui::End();
			style.pop();
		}

		template <class Archive>
		void serialize(Archive& ar) {
			EXPORT_ITEM(style);
		}

	private:
		int flags = 0;
	};
	
}