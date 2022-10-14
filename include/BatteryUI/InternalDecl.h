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

        BATTERYUI_WIDGET_DEFAULT_STYLE_DECL();  // This defines a lot of functions [WidgetConfig.h]
	}

    inline void RequestRedraw() {
        if (Internal::redrawRequestCallback)
            Internal::redrawRequestCallback();
    }
}
