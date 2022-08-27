#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/FileWatcher.h"
#include "BatteryUI/serialize.h"

namespace BatteryUI {

	struct ColorScheme {

		struct Color {
			float r = 0, g = 0, b = 0, a = 0;
			Color() {}
			Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
			Color(ImVec4 c) : r(c.x), g(c.y), b(c.z), a(c.w) {}
			operator ImVec4() {
				return { r, g, b, a };
			}

			template <class Archive>
			void serialize(Archive& ar) {
				EXPORT_ITEM(r);
				EXPORT_ITEM(g);
				EXPORT_ITEM(b);
				EXPORT_ITEM(a);
			}
		};

		struct ColorEntry {
			std::string identifier;
			Color color;

			template <class Archive>
			void serialize(Archive& ar) {
				EXPORT_ITEM(identifier);
				EXPORT_ITEM(color);
			}
		};

		size_t size() const {
			return colors.size();
		}

		void load() {
			colors.clear();
			for (size_t i = 0; i < ImGuiCol_COUNT; i++) {
				ColorScheme::ColorEntry entry;
				entry.identifier = magic_enum::enum_name((ImGuiCol_)i).data();
				entry.color = ImGui::GetStyle().Colors[i];
				colors.push_back(entry);
			}
		}

		void apply() {
			ImVec4* c = ImGui::GetStyle().Colors;

			for (auto& entry : colors) {
				size_t index = findImguiIndex(entry.identifier);
				if (index != -1)
					c[index] = entry.color;
			}
		}

		size_t findImguiIndex(const std::string& id) {
			for (size_t i = 0; i < ImGuiCol_COUNT; i++) {
				auto identifier = magic_enum::enum_name((ImGuiCol_)i);
				if (identifier == id) {
					return i;
				}
			}
			return -1;
		}

		void push() {
			for (auto& entry : colors) {
				size_t index = findImguiIndex(entry.identifier);
				if (index != -1)
					ImGui::PushStyleColor(index, entry.color);
			}
		}

		void pop() {
			for (auto& entry : colors) {
				size_t index = findImguiIndex(entry.identifier);
				if (index != -1)
					ImGui::PopStyleColor();
			}
		}

		void addColor(const std::string& identifier, float r, float g, float b, float a) {
			ColorScheme::ColorEntry entry;
			entry.identifier = identifier;
			entry.color = { r, g, b, a };
			colors.push_back(entry);
		}

		std::vector<ColorEntry> colors;

		template <class Archive>
		void serialize(Archive& ar) {
			EXPORT_ITEM(colors);
		}
	};







	struct Colors {
		std::optional<float> alpha;
		std::optional<float> disabledAlpha;

		void push() {
			PUSH_STYLE(alpha, ImGuiStyleVar_Alpha);
			PUSH_STYLE(disabledAlpha, ImGuiStyleVar_DisabledAlpha);
		}

		void pop() {
			POP_STYLE(alpha);
			POP_STYLE(disabledAlpha);
		}

		bool has_value() const {
			return alpha.has_value() || disabledAlpha.has_value();
		}
		
		template <class Archive> 
		void save(Archive& ar) const {
			SAVE_ITEM_IF_VALUE_OPT(float, alpha);
			SAVE_ITEM_IF_VALUE_OPT(float, disabledAlpha);
		}
		
		template <class Archive> 
		void load(Archive& ar) {
			LOAD_ITEM_IF_EXISTS_OPT(float, alpha);
			LOAD_ITEM_IF_EXISTS_OPT(float, disabledAlpha);
		}
	};

	struct Spacing {
		vec2_opt windowPadding;
		vec2_opt framePadding;
		vec2_opt cellPadding;
		vec2_opt itemSpacing;
		vec2_opt itemInnerSpacing;
		std::optional<float> indentSpacing;
		std::optional<float> scrollbarSize;
		std::optional<float> grabMinSize;
		
		void push() {
			PUSH_STYLE(windowPadding, ImGuiStyleVar_WindowPadding);
			PUSH_STYLE(framePadding, ImGuiStyleVar_FramePadding);
			PUSH_STYLE(cellPadding, ImGuiStyleVar_CellPadding);
			PUSH_STYLE(itemSpacing, ImGuiStyleVar_ItemSpacing);
			PUSH_STYLE(itemInnerSpacing, ImGuiStyleVar_ItemInnerSpacing);
			PUSH_STYLE(indentSpacing, ImGuiStyleVar_IndentSpacing);
			PUSH_STYLE(scrollbarSize, ImGuiStyleVar_ScrollbarSize);
			PUSH_STYLE(grabMinSize, ImGuiStyleVar_GrabMinSize);
		}

		void pop() {
			POP_STYLE(windowPadding);
			POP_STYLE(framePadding);
			POP_STYLE(cellPadding);
			POP_STYLE(itemSpacing);
			POP_STYLE(itemInnerSpacing);
			POP_STYLE(indentSpacing);
			POP_STYLE(scrollbarSize);
			POP_STYLE(grabMinSize);
		}

		bool has_value() const {
			return windowPadding.has_value() || 
				   framePadding.has_value() || 
				   cellPadding.has_value() || 
				   itemSpacing.has_value() || 
				   itemInnerSpacing.has_value() || 
				   indentSpacing.has_value() || 
				   scrollbarSize.has_value() || 
				   grabMinSize.has_value();
		}

		template <class Archive>
		void save(Archive& ar) const {
			SAVE_ITEM_IF_VALUE(windowPadding);
			SAVE_ITEM_IF_VALUE(framePadding);
			SAVE_ITEM_IF_VALUE(cellPadding);
			SAVE_ITEM_IF_VALUE(itemSpacing);
			SAVE_ITEM_IF_VALUE(itemInnerSpacing);
			SAVE_ITEM_IF_VALUE_OPT(float, indentSpacing);
			SAVE_ITEM_IF_VALUE_OPT(float, scrollbarSize);
			SAVE_ITEM_IF_VALUE_OPT(float, grabMinSize);
		}

		template <class Archive>
		void load(Archive& ar) {
			LOAD_ITEM_IF_EXISTS(windowPadding);
			LOAD_ITEM_IF_EXISTS(framePadding);
			LOAD_ITEM_IF_EXISTS(cellPadding);
			LOAD_ITEM_IF_EXISTS(itemSpacing);
			LOAD_ITEM_IF_EXISTS(itemInnerSpacing);
			LOAD_ITEM_IF_EXISTS_OPT(float, indentSpacing);
			LOAD_ITEM_IF_EXISTS_OPT(float, scrollbarSize);
			LOAD_ITEM_IF_EXISTS_OPT(float, grabMinSize);
		}
	};

	struct Borders {
		std::optional<float> windowBorderSize;
		std::optional<float> childBorderSize;
		std::optional<float> popupBorderSize;
		std::optional<float> frameBorderSize;
		
		void push() {
			PUSH_STYLE(windowBorderSize, ImGuiStyleVar_WindowBorderSize);
			PUSH_STYLE(childBorderSize, ImGuiStyleVar_ChildBorderSize);
			PUSH_STYLE(popupBorderSize, ImGuiStyleVar_PopupBorderSize);
			PUSH_STYLE(frameBorderSize, ImGuiStyleVar_FrameBorderSize);
		}

		void pop() {
			POP_STYLE(windowBorderSize);
			POP_STYLE(childBorderSize);
			POP_STYLE(popupBorderSize);
			POP_STYLE(frameBorderSize);
		}

		bool has_value() const {
			return  windowBorderSize.has_value() ||
				    childBorderSize.has_value() ||
				    popupBorderSize.has_value() ||
				    frameBorderSize.has_value();
		}

		template <class Archive>
		void save(Archive& ar) const {
			SAVE_ITEM_IF_VALUE_OPT(float, windowBorderSize);
			SAVE_ITEM_IF_VALUE_OPT(float, childBorderSize);
			SAVE_ITEM_IF_VALUE_OPT(float, popupBorderSize);
			SAVE_ITEM_IF_VALUE_OPT(float, frameBorderSize);
		}

		template <class Archive>
		void load(Archive& ar) {
			LOAD_ITEM_IF_EXISTS_OPT(float, windowBorderSize);
			LOAD_ITEM_IF_EXISTS_OPT(float, childBorderSize);
			LOAD_ITEM_IF_EXISTS_OPT(float, popupBorderSize);
			LOAD_ITEM_IF_EXISTS_OPT(float, frameBorderSize);
		}
	};

	struct Rounding {
		std::optional<float> windowRounding;
		std::optional<float> childRounding;
		std::optional<float> frameRounding;
		std::optional<float> popupRounding;
		std::optional<float> scrollbarRounding;
		std::optional<float> grabRounding;
		std::optional<float> tabRounding;
		
		void push() {
			PUSH_STYLE(windowRounding, ImGuiStyleVar_WindowRounding);
			PUSH_STYLE(childRounding, ImGuiStyleVar_ChildRounding);
			PUSH_STYLE(frameRounding, ImGuiStyleVar_FrameRounding);
			PUSH_STYLE(popupRounding, ImGuiStyleVar_PopupRounding);
			PUSH_STYLE(scrollbarRounding, ImGuiStyleVar_ScrollbarRounding);
			PUSH_STYLE(grabRounding, ImGuiStyleVar_GrabRounding);
			PUSH_STYLE(tabRounding, ImGuiStyleVar_TabRounding);
		}

		void pop() {
			POP_STYLE(windowRounding);
			POP_STYLE(childRounding);
			POP_STYLE(frameRounding);
			POP_STYLE(popupRounding);
			POP_STYLE(scrollbarRounding);
			POP_STYLE(grabRounding);
			POP_STYLE(tabRounding);
		}

		bool has_value() const {
			return  windowRounding.has_value() ||
					childRounding.has_value() ||
					frameRounding.has_value() ||
					popupRounding.has_value() ||
					scrollbarRounding.has_value() ||
					grabRounding.has_value() ||
					tabRounding.has_value();
		}

		template <class Archive>
		void save(Archive& ar) const {
			SAVE_ITEM_IF_VALUE_OPT(float, windowRounding);
			SAVE_ITEM_IF_VALUE_OPT(float, childRounding);
			SAVE_ITEM_IF_VALUE_OPT(float, frameRounding);
			SAVE_ITEM_IF_VALUE_OPT(float, popupRounding);
			SAVE_ITEM_IF_VALUE_OPT(float, scrollbarRounding);
			SAVE_ITEM_IF_VALUE_OPT(float, grabRounding);
			SAVE_ITEM_IF_VALUE_OPT(float, tabRounding);
		}

		template <class Archive>
		void load(Archive& ar) {
			LOAD_ITEM_IF_EXISTS_OPT(float, windowRounding);
			LOAD_ITEM_IF_EXISTS_OPT(float, childRounding);
			LOAD_ITEM_IF_EXISTS_OPT(float, frameRounding);
			LOAD_ITEM_IF_EXISTS_OPT(float, popupRounding);
			LOAD_ITEM_IF_EXISTS_OPT(float, scrollbarRounding);
			LOAD_ITEM_IF_EXISTS_OPT(float, grabRounding);
			LOAD_ITEM_IF_EXISTS_OPT(float, tabRounding);
		}
	};

	struct Alignment {
		std::optional<float> windowTitleAlignment;
		std::optional<float> buttonTextAlign;
		std::optional<float> selectableTextAlign;

		void push() {
			PUSH_STYLE(windowTitleAlignment, ImGuiStyleVar_WindowTitleAlign);
			PUSH_STYLE(buttonTextAlign, ImGuiStyleVar_ButtonTextAlign);
			PUSH_STYLE(selectableTextAlign, ImGuiStyleVar_SelectableTextAlign);
		}

		void pop() {
			POP_STYLE(windowTitleAlignment);
			POP_STYLE(buttonTextAlign);
			POP_STYLE(selectableTextAlign);
		}

		bool has_value() const {
			return windowTitleAlignment.has_value() ||
				   buttonTextAlign.has_value() ||
				   selectableTextAlign.has_value();
		}

		template <class Archive>
		void save(Archive& ar) const {
			SAVE_ITEM_IF_VALUE_OPT(float, windowTitleAlignment);
			SAVE_ITEM_IF_VALUE_OPT(float, buttonTextAlign);
			SAVE_ITEM_IF_VALUE_OPT(float, selectableTextAlign);
		}

		template <class Archive>
		void load(Archive& ar) {
			LOAD_ITEM_IF_EXISTS_OPT(float, windowTitleAlignment);
			LOAD_ITEM_IF_EXISTS_OPT(float, buttonTextAlign);
			LOAD_ITEM_IF_EXISTS_OPT(float, selectableTextAlign);
		}
	};

	struct Style {
		Colors colors;
		Spacing spacing;
		Borders borders;
		Rounding rounding;
		Alignment alignment;
		
		void push() {
			colors.push();
			spacing.push();
			borders.push();
			rounding.push();
			alignment.push();
		}
		
		void pop() {
			colors.pop();
			spacing.pop();
			borders.pop();
			rounding.pop();
			alignment.pop();
		}

		bool has_value() const {
			return colors.has_value() ||
				   spacing.has_value() ||
				   borders.has_value() ||
				   rounding.has_value() ||
				   alignment.has_value();
		}

		template <class Archive>
		void save(Archive& ar) const {
			SAVE_ITEM_IF_VALUE(colors);
			SAVE_ITEM_IF_VALUE(spacing);
			SAVE_ITEM_IF_VALUE(borders);
			SAVE_ITEM_IF_VALUE(rounding);
			SAVE_ITEM_IF_VALUE(alignment);
		}

		template <class Archive>
		void load(Archive& ar) {
			LOAD_ITEM_IF_EXISTS(colors);
			LOAD_ITEM_IF_EXISTS(spacing);
			LOAD_ITEM_IF_EXISTS(borders);
			LOAD_ITEM_IF_EXISTS(rounding);
			LOAD_ITEM_IF_EXISTS(alignment);
		}
	};
	
}
