#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/Style.h"
#include "BatteryUI/RedrawNotifier.h"

#include "BatteryUI/Widgets/Window.h"
#include "BatteryUI/Widgets/Button.h"
#include "BatteryUI/Widgets/Dropdown.h"
#include "BatteryUI/Widgets/HorizontalGrid.h"
#include "BatteryUI/Widgets/VerticalGrid.h"

#define HOTRELOAD_UPDATE_INTERVAL_MS 1000

namespace BatteryUI {

	struct DefaultStyles {
		
		ImGuiStyle imguiStyle;
		ImGuiColors imguiColors;
		
		ButtonStyle button;
		DropdownStyle dropdown;
        ContainerStyle container;
        HorizontalGridStyle horizontalGrid;
        VerticalGridStyle verticalGrid;

		void push() {       // The Widgets' default styles are not pushed here
			imguiStyle.push();
			imguiColors.push();
		}

		void pop() {
            imguiColors.pop();
            imguiStyle.pop();
		}

		DefaultStyles() {
            imguiStyle = ImGuiStyle();
            imguiColors = ImGuiColors();

			Button::Presets::load();
			button = Button::Presets::Modern;

			Dropdown::Presets::load();
			dropdown = Dropdown::Presets::Modern;

            Container::Presets::load();
            container = Container::Presets::Thin;

            HorizontalGrid::Presets::load();
            horizontalGrid = HorizontalGrid::Presets::Hidden;

            VerticalGrid::Presets::load();
            verticalGrid = VerticalGrid::Presets::Hidden;
		}
	};

    class StyleManagerWindow : public BatteryUI::Window {
    public:
        BatteryUI::Window window;
        BatteryUI::Button btnSave;
        BatteryUI::Button btnLoad;

        StyleManagerWindow() {
            btnSave.name = "Save";
            btnLoad.name = "Load";
        }

        void operator()() {
            window([&] {
                btnSave();
                btnLoad();
            });
        }
    };
	
	class RootUI {
	public:
		inline static DefaultStyles defaultStyle;

		explicit RootUI(const std::string& styleSheet)
          : fileWatcher(styleSheet) {
			this->styleSheet = styleSheet;
            defaultStyle = DefaultStyles();
			window.name = "Style Manager";

            watcherThread = std::thread([&] {
                while (!terminate) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(period));
                    if (fileWatcher.update()) {
                        loadStyleSheet();
                        if (Internal::redrawRequestCallback) {
                            Internal::redrawRequestCallback();
                        }
                    }
                }
            });
		}

        ~RootUI() {
            terminate = true;
            watcherThread.join();
        }
		
		bool loadStyleSheet() {     // TODO: Prevent infinite hang-up
			do {
				std::ifstream file(styleSheet);
				if (!file.is_open()) {
					printf("[%s]: Failed to load style sheet: No such file or directory. Creating default stylesheet\n", __FUNCTION__);
					break;
				}

				try {
                    std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
                    applyJsonRootUI(nlohmann::json::parse(content));
				}
				catch (const std::exception& e) {
					file.close();
					printf("[%s]: Error while loading style sheet: %s\n", __FUNCTION__, e.what());
					return false;
				}

				file.close();
				return true;
			} while (false);

			saveStyleSheet();		// Default create -> The std::ifstream object must be destructed before calling this
			return loadStyleSheet();
		}

        virtual void applyJsonRootUI(const nlohmann::json& json) = 0;
		
		void saveStyleSheet() {
			std::ofstream file(styleSheet);
			if (!file.is_open()) {
				printf("[%s]: Failed to open style sheet for writing: No such file or directory.\n", __FUNCTION__);
				return;
			}

			try {
                file << getJsonRootUI().dump(4);
			}
			catch (const std::exception& e) {
				printf("[%s]: Error writing style sheet: %s\n", __FUNCTION__, e.what());
			}

			file.close();
		}

        virtual nlohmann::json getJsonRootUI() = 0;

		void drawStyleManagerWindow() {
            styleManagerWindow();

            if (styleManagerWindow.btnSave.clicked) {
                std::cout << "Saving style sheet" << std::endl;
                saveStyleSheet();
            }
            if (styleManagerWindow.btnLoad.clicked) {
                std::cout << "Loading style sheet" << std::endl;
                loadStyleSheet();
            }
		}

    private:
        StyleManagerWindow styleManagerWindow;

	private:
		std::string styleSheet;

        std::atomic<bool> terminate = false;
        int64_t period = HOTRELOAD_UPDATE_INTERVAL_MS;
        std::thread watcherThread;
		FileWatcher fileWatcher;

		Window window;
	};
	
}
