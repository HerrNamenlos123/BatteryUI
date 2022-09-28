#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/Style.h"
#include "BatteryUI/InternalDecl.h"

namespace BatteryUI {
	
	class BasicWidget {
	public:
		Style style;
		bool sameline = false;
		std::string name;

		BasicWidget() {
			newID();
		}

		BasicWidget(BasicWidget const& other) = delete;
		void operator=(BasicWidget const& other) = delete;

		BasicWidget(BasicWidget&& other) = default;	// Moving is allowed as the other object is then invalid
		BasicWidget& operator=(BasicWidget&& other) = default;

	protected:
		std::string getIdentifier() {
			return name + "##BatteryUI" + std::to_string(id);
		}
		size_t id = 0;

	private:
		void newID() {
			id = __id_counter;
			__id_counter++;
		}

		inline static size_t __id_counter = 0;
	};
	
}
