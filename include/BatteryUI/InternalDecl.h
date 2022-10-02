#pragma once

#include "BatteryUI/common.h"

namespace BatteryUI {

	struct ButtonStyle;
	struct DropdownStyle;
	struct ChildStyle;
	struct HorizontalGridStyle;
	struct VerticalGridStyle;

	namespace Internal {

		inline std::function<void(void)> redrawRequestCallback;

        inline void ApplyColorScheme();
        inline void LoadColorScheme();
		
		inline ButtonStyle* GetButtonDefaultStyle();
		inline DropdownStyle* GetDropdownDefaultStyle();
        inline ChildStyle* GetChildDefaultStyle();
        inline HorizontalGridStyle* GetHorizontalGridDefaultStyle();
        inline VerticalGridStyle* GetVerticalGridDefaultStyle();

	}
}
