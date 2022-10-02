#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/RootUI.h"

#include "BatteryUI/Widgets/Button.h"
#include "BatteryUI/Widgets/Dropdown.h"

namespace BatteryUI {
	namespace Internal {
		
        inline void ApplyColorScheme() {
            //RootUI::defaults.colorScheme.apply();
        }
		
        inline void LoadColorScheme() {
            //RootUI::defaults.colorScheme.load();
        }


		
        inline ButtonStyle* GetButtonDefaultStyle() {
            return &RootUI::defaults.button;
        }

        inline DropdownStyle* GetDropdownDefaultStyle() {
            return &RootUI::defaults.dropdown;
        }

        inline ContainerStyle* GetChildDefaultStyle() {
            return &RootUI::defaults.container;
        }

        inline HorizontalGridStyle* GetHorizontalGridDefaultStyle() {
            return &RootUI::defaults.horizontalGrid;
        }

        inline VerticalGridStyle* GetVerticalGridDefaultStyle() {
            return &RootUI::defaults.verticalGrid;
        }

	}
}
