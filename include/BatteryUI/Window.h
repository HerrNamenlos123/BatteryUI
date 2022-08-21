#pragma once

#include <string>
#include <functional>

#include "imgui.h"

namespace BatteryUI {
	
	class Window {
	public:
		std::string name = "BatteryUI Window";
		
		Window() {
			newID();
		}
		
		Window(const std::string& name, int flags = 0) {
			newID();
			this->name = name;
			this->flags = flags;
		}

		void draw(std::function<void(void)> callback) {
			ImGui::PushID(id);
			ImGui::Begin(name.c_str(), nullptr, flags);

			callback();		// Do the user rendering here
			
			ImGui::End();
			ImGui::PopID();
		}
		
		Window(Window const& other) {		// New id but copied data
			newID();
			this->name = other.name;
			this->flags = other.flags;
		}
		
		void operator=(Window const& other) {	// Unchanged id but copied data
			this->name = other.name;
			this->flags = other.flags;
		}

		Window(Window&& other) = default;	// Moving is allowed as the other object is then invalid
		Window& operator=(Window&& other) = default;

	private:
		void newID() {
			id = __id_counter;
			__id_counter++;
		}

	private:
		int flags = 0;

		size_t id = 0;
		inline static size_t __id_counter = 0;
	};
	
}