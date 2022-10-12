#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/RootUI.h"

#include "BatteryUI/Widgets/Button.h"
#include "BatteryUI/Widgets/Dropdown.h"

namespace BatteryUI {
	namespace Internal {

        inline ButtonStyle* GetButtonDefaultStyle() {
            return &RootUI::defaultStyle.button;
        }

        inline DropdownStyle* GetDropdownDefaultStyle() {
            return &RootUI::defaultStyle.dropdown;
        }

        inline ContainerStyle* GetChildDefaultStyle() {
            return &RootUI::defaultStyle.container;
        }

        inline HorizontalGridStyle* GetHorizontalGridDefaultStyle() {
            return &RootUI::defaultStyle.horizontalGrid;
        }

        inline VerticalGridStyle* GetVerticalGridDefaultStyle() {
            return &RootUI::defaultStyle.verticalGrid;
        }

	}
}
