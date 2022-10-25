#pragma once

#include "BatteryUI/Style.h"

namespace BatteryUI {

    class Theme {
    public:
        Theme() = default;

        WidgetStyle style;
    };

    class Theme_Modern : public Theme {
    public:
        Theme_Modern() {

            // Window styling
            style["ImGuiCol_WindowBg"] = "#2e2e2e";

            // Container Widget styling
            style["ContainerBorderWidth"] = 1;
            style["ContainerBorderRadius"] = 5;
            style["ContainerBorderColor"] = "#FF0000";

            // Button Widget styling
            style["ButtonWidth"] = -1;
            style["ButtonHeight"] = 40;
            style["ButtonBaseColor"] = "#424242";
            style["ButtonHoverColor"] = "#ce881b";
            style["ButtonActiveColor"] = "#dcb880";
            style["ButtonBorderWidth"] = 0.f;
            style["ButtonBorderRadius"] = 5;
            style["ButtonBorderColor"] = "#697573";
        }
    };

}
