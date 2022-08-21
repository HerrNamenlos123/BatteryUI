#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/Font.h"

namespace BatteryUI {

    template<typename T, typename... TArgs>
    inline std::unique_ptr<T> Setup(TArgs... args) {
        return std::make_unique<T>(args...);
    }

    inline void NewFrame() {

    }

    inline void EndFrame() {
        
    }

    template<typename T>
    inline void Shutdown(T& ui) {
        ui.reset();
    }

}
