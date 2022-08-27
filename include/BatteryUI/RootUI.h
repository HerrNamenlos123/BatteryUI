#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/Style.h"
#include "BatteryUI/Window.h"

#include "BatteryUI/Button.h"

namespace BatteryUI {

	struct DefaultStyles {
		
		Style style;
		ColorScheme colorScheme;
		ButtonDefaultStyle button;

		template <class Archive>
		void serialize(Archive& ar) {
			EXPORT_ITEM(style);
			EXPORT_ITEM(colorScheme);
			EXPORT_ITEM(button);
		}
	};
	
	class RootUI {
	public:
		inline static DefaultStyles defaults;

		RootUI(const std::string& styleSheet) : watcher(styleSheet) {
			this->styleSheet = styleSheet;
			window.name = "Style Manager";
		}

		void updateHotreload() {
			bool modified = watcher.update();
			if (modified) {
				loadStyleSheet();
			}
		}
		
		bool loadStyleSheet() {
			std::ifstream file(styleSheet);
			if (!file.is_open()) {
				printf("[%s]: Failed to load style sheet: No such file or directory. Creating default stylesheet\n", __FUNCTION__);
				saveStyleSheet();		// Default create
				return false;
			}

			try {
				cereal::JSONInputArchive ar(file);
				archive(ar);
			}
			catch (const std::exception& e) {
				file.close();
				printf("[%s]: Error while loading style sheet: %s\n", __FUNCTION__, e.what());
				return false;
			}

			file.close();
			Internal::ApplyColorScheme();
			return true;
		}
		
		void saveStyleSheet() {
			Internal::LoadColorScheme();
			std::ofstream file(styleSheet);
			if (!file.is_open()) {
				printf("[%s]: Failed to load style sheet: No such file or directory.\n", __FUNCTION__);
				return;
			}

			try {
				cereal::JSONOutputArchive ar(file);
				archive(ar);
			}
			catch (const std::exception& e) {
				printf("[%s]: Error while saving style sheet: %s\n", __FUNCTION__, e.what());
			}

			file.close();
		}

		void drawStyleManagerWindow() {
			window.draw([&] {
				ImGui::Text("Hello worlds");
				if (ImGui::Button("Save##lol")) {
					saveStyleSheet();
				}
				if (ImGui::Button("Load##lol")) {
					loadStyleSheet();
				}
			});
		}

		virtual void archive(cereal::JSONInputArchive& ar) = 0;
		virtual void archive(cereal::JSONOutputArchive& ar) = 0;

	private:
		std::string styleSheet;
		FileWatcher watcher;

		Window window;
	};
	
}
