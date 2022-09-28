#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/InternalDecl.h"

namespace BatteryUI {

	class RedrawNotifier {
	public:
		RedrawNotifier(int64_t redrawPeriodMS) : period(redrawPeriodMS) {
			thread = std::thread([&] { update(); });
		}

		~RedrawNotifier() {
			this->terminate = true;
			thread.join();
		}

	private:
		void update() {
			while (!this->terminate) {
				std::this_thread::sleep_for(std::chrono::milliseconds(period));
				if (Internal::redrawRequestCallback) {
					Internal::redrawRequestCallback();
				}
			}
		}

		int64_t period = 0;
		std::thread thread;
		std::atomic<bool> terminate = false;
	};

}
