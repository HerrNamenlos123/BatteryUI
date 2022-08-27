#pragma once

#include "BatteryUI/common.h"

namespace BatteryUI {

	struct vec2_opt;
	
	namespace Internal {

		inline void PushButtonDefaultStyle(bool& half_round);
        inline void PopButtonDefaultStyle();
        inline void ApplyColorScheme();
        inline void LoadColorScheme();
        inline vec2_opt& GetButtonDefaultSize();
		
	}
}
