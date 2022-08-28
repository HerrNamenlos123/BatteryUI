#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/Font.h"
#include "BatteryUI/RootUI.h"
#include "BatteryUI/InternalImpl.h"

#include <memory>

namespace BatteryUI {

    template<typename T, typename... TArgs>
    inline std::unique_ptr<T> Setup(TArgs... args) {
        auto ui = std::make_unique<T>(args...);
        ui->loadStyleSheet();
        ui->updateHotreload();
        return std::move(ui);
    }

    inline void NewFrame() {
        RootUI::defaults.push();
    }

    inline void EndFrame() {
        RootUI::defaults.pop();
    }

    template<typename T>
    inline void Shutdown(T& ui) {
        ui->saveStyleSheet();
        ui.reset();
    }

}
