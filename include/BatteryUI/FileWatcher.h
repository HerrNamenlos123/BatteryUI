#pragma once

#include "BatteryUI/common.h"

namespace BatteryUI {
	
	class FileWatcher {
	public:
		FileWatcher(const std::string& filename, float updateInterval = 0.1f) {
			this->filename = filename;
			this->updateInterval = updateInterval;
			path = std::filesystem::path(filename);
		}

		bool update() {
			auto now = std::chrono::steady_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - lastUpdateTime).count();
			if (elapsed <= updateInterval) {
				return false;
			}
			lastUpdateTime = now;
			
			try {
				auto modificationTime = std::filesystem::last_write_time(path);
				if (modificationTime != lastModifyTime) {
					lastModifyTime = modificationTime;
					return true;
				}
			}
			catch (...) {}
			return false;
		}

	private:
		float updateInterval = 0;
		std::string filename;
		std::filesystem::path path;
		std::filesystem::file_time_type lastModifyTime;
		std::chrono::steady_clock::time_point lastUpdateTime;
	};
}
