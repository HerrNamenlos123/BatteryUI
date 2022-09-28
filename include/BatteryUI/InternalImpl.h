#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/RootUI.h"

#include "BatteryUI/Widgets/Button.h"
#include "BatteryUI/Widgets/Dropdown.h"

namespace BatteryUI {
	namespace Internal {
		
        inline void ApplyColorScheme() {
            RootUI::defaults.colorScheme.apply();
        }
		
        inline void LoadColorScheme() {
            RootUI::defaults.colorScheme.load();
        }


		
        inline ButtonDefaultStyle* GetButtonDefaults() {
            return &RootUI::defaults.button;
        }

        inline DropdownDefaultStyle* GetDropdownDefaults() {
            return &RootUI::defaults.dropdown;
        }

	}
}
