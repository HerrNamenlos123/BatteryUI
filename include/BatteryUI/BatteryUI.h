#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/Font.h"
#include "BatteryUI/RootUI.h"
#include "BatteryUI/InternalImpl.h"

#include <memory>

namespace BatteryUI {

    template<typename T, typename... TArgs>
    inline std::unique_ptr<T> Setup(TArgs... args) {
        return std::make_unique<T>(args...);
    }

    inline void NewFrame() {
        RootUI::defaults.style.push();
    }

    inline void EndFrame() {
        RootUI::defaults.style.pop();
    }

    template<typename T>
    inline void Shutdown(T& ui) {
        ui.reset();
    }

}
