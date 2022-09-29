#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/Style.h"
#include "BatteryUI/InternalDecl.h"

namespace BatteryUI {

    // Every class derived from BasicWidget gets a unique ID,
    // which is pushed to ImGui for context separation
	
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
		[[nodiscard]] std::string getIdentifier() const {
			return name + "##BatteryUI" + std::to_string(id);
		}
		size_t id = 0;

	private:
		void newID() {
			id = _id_counter;
			_id_counter++;
		}

		inline static size_t _id_counter = 0;
	};
	
}
