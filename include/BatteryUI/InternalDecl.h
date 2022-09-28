#pragma once

#include "BatteryUI/common.h"

namespace BatteryUI {

	struct vec2_opt;
	struct ButtonDefaultStyle;
	struct DropdownDefaultStyle;
	
	namespace Internal {

		inline std::function<void(void)> redrawRequestCallback;

        inline void ApplyColorScheme();
        inline void LoadColorScheme();
		
		inline ButtonDefaultStyle* GetButtonDefaults();
		inline DropdownDefaultStyle* GetDropdownDefaults();

	}
}
