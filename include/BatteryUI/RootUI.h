#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/Style.h"
#include "BatteryUI/Window.h"

#include "BatteryUI/Button.h"

namespace BatteryUI {

	struct DefaultStyles {
		
		Style style;
		ColorScheme colorScheme;
		
		ButtonDefaultStyle button;	// Widgets are not pushed here, they are pushed per-item

		void push() {
			style.push();
			colorScheme.push();
		}

		void pop() {
			style.pop();
			colorScheme.pop();
		}

		template <class Archive>
		void serialize(Archive& ar) {
			EXPORT_ITEM(style);
			EXPORT_ITEM(colorScheme);
			EXPORT_ITEM(button);
		}

		DefaultStyles() {
			style = Style();
			
			colorScheme = ColorScheme();

			Button::Presets::load();
			button = ButtonDefaultStyle(Button::Presets::Modern);
		}
	};
	
	class RootUI {
	public:
		inline static DefaultStyles defaults;

		RootUI(const std::string& styleSheet) : watcher(styleSheet) {
			this->styleSheet = styleSheet;
			defaults = DefaultStyles();
			window.name = "Style Manager";
		}

		void updateHotreload() {
			bool modified = watcher.update();
			if (modified) {
				loadStyleSheet();
			}
		}
		
		bool loadStyleSheet() {
			do {
				std::ifstream file(styleSheet);
				if (!file.is_open()) {
					printf("[%s]: Failed to load style sheet: No such file or directory. Creating default stylesheet\n", __FUNCTION__);
					break;
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
			} while (false);

			saveStyleSheet();		// Default create -> The std::ifstream object must be destructed before calling this
			return loadStyleSheet();
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
