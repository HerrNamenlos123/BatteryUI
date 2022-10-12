#pragma once

#include "BatteryUI/common.h"

namespace BatteryUI {

	struct ButtonStyle;
	struct DropdownStyle;
	struct ContainerStyle;
	struct HorizontalGridStyle;
	struct VerticalGridStyle;

	namespace Internal {

		inline BatteryUI::Callback redrawRequestCallback;
		
		inline ButtonStyle* GetButtonDefaultStyle();
		inline DropdownStyle* GetDropdownDefaultStyle();
        inline ContainerStyle* GetChildDefaultStyle();
        inline HorizontalGridStyle* GetHorizontalGridDefaultStyle();
        inline VerticalGridStyle* GetVerticalGridDefaultStyle();

	}
}
