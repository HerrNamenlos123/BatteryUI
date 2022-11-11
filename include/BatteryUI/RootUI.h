#pragma once

#include <utility>

#include "BatteryUI/common.h"
#include "BatteryUI/Style.h"
#include "BatteryUI/RedrawNotifier.h"
#include "BatteryUI/InternalDecl.h"

#include "BatteryUI/Widgets/Window.h"
#include "BatteryUI/Widgets/Button.h"
#include "BatteryUI/Widgets/Dropdown.h"
#include "BatteryUI/Widgets/HorizontalGrid.h"
#include "BatteryUI/Widgets/VerticalGrid.h"

#define HOTRELOAD_UPDATE_INTERVAL_MS 1000

namespace BatteryUI {

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

        BATTERYUI_SERIALIZE(StyleManagerWindow, window, btnSave, btnLoad);
    };
	
	class RootUI {
	public:

		virtual void setupHotreload() {}
		virtual void terminateHotreload() {}
		virtual void applyJsonRootUI(const nlohmann::json& json) = 0;
		virtual void getJsonRootUI(nlohmann::json& json) = 0;

		explicit RootUI(std::string  styleSheet) : styleSheet(std::move(styleSheet)) {
			window.name = "Style Manager";
			setupHotreload();
		}
		
		bool loadStyleSheet() {     // TODO: Prevent infinite hang-up
			std::cout << "Loading style sheet" << std::endl;
			
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
					printf("[%s]: Error while loading style sheet: %s\n", __FUNCTION__, e.what());
					return false;
				}
				
				return true;
			} while (false);

			saveStyleSheet();		// Default create -> The std::ifstream object must be destructed before calling this
			return loadStyleSheet();
		}
		
		void saveStyleSheet() {
			std::cout << "Saving style sheet" << std::endl;
			
			std::ofstream file(styleSheet);
			if (!file.is_open()) {
				printf("[%s]: Failed to open style sheet for writing: No such file or directory.\n", __FUNCTION__);
				return;
			}

			try {
				nlohmann::json json;
				getJsonRootUI(json);
                file << json.dump(4);
			}
			catch (const std::exception& e) {
				printf("[%s]: Error writing style sheet: %s\n", __FUNCTION__, e.what());
			}
		}

		void drawStyleManagerWindow() {
            styleManagerWindow();

            if (styleManagerWindow.btnSave.clicked) {
                saveStyleSheet();
            }
            if (styleManagerWindow.btnLoad.clicked) {
                loadStyleSheet();
            }
		}

    public:
        StyleManagerWindow styleManagerWindow;
		std::string styleSheet;

	private:
		Window window;
	};

	class HotreloadHandler {
	public:
		explicit HotreloadHandler(RootUI* _ui, int64_t _period_ms = HOTRELOAD_UPDATE_INTERVAL_MS) {
			this->ui = _ui;
			this->period_ms = _period_ms;
			this->fileWatcher = std::make_unique<FileWatcher>(ui->styleSheet);

			watcherThread = std::thread([&] {
				while (!terminateWatcher) {
                    wait(std::chrono::milliseconds(period_ms));
					if (fileWatcher->update()) {
						ui->loadStyleSheet();
						BatteryUI::Internal::callbacks.callback_requestRedraw();
					}
				}
				});
		}

		~HotreloadHandler() {
			terminateWatcher = true;
            wait.terminate();
			watcherThread.join();
		}
		
	private:
		std::atomic<bool> terminateWatcher = false;
		std::thread watcherThread;
        AsyncWait wait;
		
		int64_t period_ms = 0;
		std::unique_ptr<FileWatcher> fileWatcher;
		std::string stylesheet;

		RootUI* ui = nullptr;
	};

#define BATTERYUI_STYLESHEET_HOTRELOAD(...) \
	std::unique_ptr<BatteryUI::HotreloadHandler> __hotreload; \
	void setupHotreload() override { \
		__hotreload = std::make_unique<BatteryUI::HotreloadHandler>(this, ##__VA_ARGS__); \
	} \
	void terminateHotreload() override { \
		__hotreload.reset(); \
	}

#define BATTERYUI_ROOT_UI() \
    void applyJsonRootUI(const nlohmann::json& json) override { \
        from_json(json, *this); \
    } \
    void getJsonRootUI(nlohmann::json& json) override { \
        to_json(json, *this); \
    }
	
}
