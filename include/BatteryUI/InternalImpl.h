#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/RootUI.h"
#include "BatteryUI/Button.h"

namespace BatteryUI {
	namespace Internal {

        inline void PushButtonDefaultStyle(bool& half_round) {
            RootUI::defaults.button.push(half_round);
        }

        inline void PopButtonDefaultStyle() {
            RootUI::defaults.button.pop();
        }
		
        inline void ApplyColorScheme() {
            RootUI::defaults.colorScheme.apply();
        }
		
        inline void LoadColorScheme() {
            RootUI::defaults.colorScheme.load();
        }
		
        inline vec2_opt& GetButtonDefaultSize() {
            return RootUI::defaults.button.size;
        }

	}
}
