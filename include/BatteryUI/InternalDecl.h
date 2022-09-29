#pragma once

#include "BatteryUI/common.h"

namespace BatteryUI {

	struct ButtonStyle;
	struct DropdownStyle;
	
	namespace Internal {

		inline std::function<void(void)> redrawRequestCallback;

        inline void ApplyColorScheme();
        inline void LoadColorScheme();
		
		inline ButtonStyle* GetButtonDefaults();
		inline DropdownStyle* GetDropdownDefaults();

	}
}
