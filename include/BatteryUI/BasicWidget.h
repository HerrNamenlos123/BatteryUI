#pragma once

#include "BatteryUI/common.h"

namespace BatteryUI {
	
	class BasicWidget {
	public:
		Style style;
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

	private:
		void newID() {
			id = __id_counter;
			__id_counter++;
		}

		size_t id = 0;
		inline static size_t __id_counter = 0;
	};
	
}
