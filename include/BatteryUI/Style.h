#pragma once

#include "BatteryUI/common.h"
#include "BatteryUI/FileWatcher.h"
#include "BatteryUI/Property.h"

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
		};

		struct ColorEntry {
			std::string identifier;
			Color color;
		};

		[[nodiscard]] size_t size() const {
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
	};




	struct ImGuiStyle {

        ImGuiPropFloat<ImGuiStyleVar_Alpha> alpha;
        ImGuiPropFloat<ImGuiStyleVar_DisabledAlpha> disabledAlpha;

        ImGuiPropVec2<ImGuiStyleVar_WindowPadding> windowPadding;
        ImGuiPropFloat<ImGuiStyleVar_WindowRounding> windowRounding;
        ImGuiPropFloat<ImGuiStyleVar_WindowBorderSize> windowBorderSize;
        ImGuiPropVec2<ImGuiStyleVar_WindowMinSize> windowMinSize;
        ImGuiPropFloat<ImGuiStyleVar_WindowTitleAlign> windowTitleAlignment;

        ImGuiPropFloat<ImGuiStyleVar_ChildRounding> childRounding;
        ImGuiPropFloat<ImGuiStyleVar_ChildBorderSize> childBorderSize;

        ImGuiPropFloat<ImGuiStyleVar_PopupRounding> popupRounding;
        ImGuiPropFloat<ImGuiStyleVar_PopupBorderSize> popupBorderSize;

        ImGuiPropVec2<ImGuiStyleVar_FramePadding> framePadding;
        ImGuiPropFloat<ImGuiStyleVar_FrameRounding> frameRounding;
        ImGuiPropFloat<ImGuiStyleVar_FrameBorderSize> frameBorderSize;

        ImGuiPropVec2<ImGuiStyleVar_ItemSpacing> itemSpacing;
        ImGuiPropVec2<ImGuiStyleVar_ItemInnerSpacing> itemInnerSpacing;
        ImGuiPropFloat<ImGuiStyleVar_IndentSpacing> indentSpacing;

        ImGuiPropVec2<ImGuiStyleVar_CellPadding> cellPadding;
        ImGuiPropFloat<ImGuiStyleVar_ScrollbarSize> scrollbarSize;
        ImGuiPropFloat<ImGuiStyleVar_ScrollbarRounding> scrollbarRounding;

        ImGuiPropFloat<ImGuiStyleVar_GrabMinSize> grabMinSize;
        ImGuiPropFloat<ImGuiStyleVar_GrabRounding> grabRounding;
        ImGuiPropFloat<ImGuiStyleVar_TabRounding> tabRounding;

        ImGuiPropVec2<ImGuiStyleVar_ButtonTextAlign> buttonTextAlign;
        ImGuiPropVec2<ImGuiStyleVar_SelectableTextAlign> selectableTextAlign;
		
		void push() {
            alpha.push();
            disabledAlpha.push();
            windowPadding.push();
            windowRounding.push();
            windowBorderSize.push();
            windowMinSize.push();
            windowTitleAlignment.push();
            childRounding.push();
            childBorderSize.push();
            popupRounding.push();
            popupBorderSize.push();
            framePadding.push();
            frameRounding.push();
            frameBorderSize.push();
            itemSpacing.push();
            itemInnerSpacing.push();
            indentSpacing.push();
            cellPadding.push();
            scrollbarSize.push();
            scrollbarRounding.push();
            grabMinSize.push();
            grabRounding.push();
            tabRounding.push();
            buttonTextAlign.push();
            selectableTextAlign.push();
		}
		
		void pop() {
            alpha.pop();
            disabledAlpha.pop();
            windowPadding.pop();
            windowRounding.pop();
            windowBorderSize.pop();
            windowMinSize.pop();
            windowTitleAlignment.pop();
            childRounding.pop();
            childBorderSize.pop();
            popupRounding.pop();
            popupBorderSize.pop();
            framePadding.pop();
            frameRounding.pop();
            frameBorderSize.pop();
            itemSpacing.pop();
            itemInnerSpacing.pop();
            indentSpacing.pop();
            cellPadding.pop();
            scrollbarSize.pop();
            scrollbarRounding.pop();
            grabMinSize.pop();
            grabRounding.pop();
            tabRounding.pop();
            buttonTextAlign.pop();
            selectableTextAlign.pop();
		}
	};

    // JSON serialization
#define IMGUISTYLE_TO_JSON(var) if (p.var.has_value()) j[#var] = p.var;
#define IMGUISTYLE_FROM_JSON(var) try { j[#var].get_to(p.var); } catch (...) {}

    inline void to_json(nlohmann::json& j, const ImGuiStyle& p) {
        IMGUISTYLE_TO_JSON(alpha);
        IMGUISTYLE_TO_JSON(disabledAlpha);
        IMGUISTYLE_TO_JSON(windowPadding);
        IMGUISTYLE_TO_JSON(windowRounding);
        IMGUISTYLE_TO_JSON(windowBorderSize);
        IMGUISTYLE_TO_JSON(windowMinSize);
        IMGUISTYLE_TO_JSON(windowTitleAlignment);
        IMGUISTYLE_TO_JSON(childRounding);
        IMGUISTYLE_TO_JSON(childBorderSize);
        IMGUISTYLE_TO_JSON(popupRounding);
        IMGUISTYLE_TO_JSON(popupBorderSize);
        IMGUISTYLE_TO_JSON(framePadding);
        IMGUISTYLE_TO_JSON(frameRounding);
        IMGUISTYLE_TO_JSON(frameBorderSize);
        IMGUISTYLE_TO_JSON(itemSpacing);
        IMGUISTYLE_TO_JSON(itemInnerSpacing);
        IMGUISTYLE_TO_JSON(indentSpacing);
        IMGUISTYLE_TO_JSON(cellPadding);
        IMGUISTYLE_TO_JSON(scrollbarSize);
        IMGUISTYLE_TO_JSON(scrollbarRounding);
        IMGUISTYLE_TO_JSON(grabMinSize);
        IMGUISTYLE_TO_JSON(grabRounding);
        IMGUISTYLE_TO_JSON(tabRounding);
        IMGUISTYLE_TO_JSON(buttonTextAlign);
        IMGUISTYLE_TO_JSON(selectableTextAlign);
    }

    inline void from_json(const nlohmann::json& j, ImGuiStyle& p) {
        IMGUISTYLE_FROM_JSON(alpha);
        IMGUISTYLE_FROM_JSON(disabledAlpha);
        IMGUISTYLE_FROM_JSON(windowPadding);
        IMGUISTYLE_FROM_JSON(windowRounding);
        IMGUISTYLE_FROM_JSON(windowBorderSize);
        IMGUISTYLE_FROM_JSON(windowMinSize);
        IMGUISTYLE_FROM_JSON(windowTitleAlignment);
        IMGUISTYLE_FROM_JSON(childRounding);
        IMGUISTYLE_FROM_JSON(childBorderSize);
        IMGUISTYLE_FROM_JSON(popupRounding);
        IMGUISTYLE_FROM_JSON(popupBorderSize);
        IMGUISTYLE_FROM_JSON(framePadding);
        IMGUISTYLE_FROM_JSON(frameRounding);
        IMGUISTYLE_FROM_JSON(frameBorderSize);
        IMGUISTYLE_FROM_JSON(itemSpacing);
        IMGUISTYLE_FROM_JSON(itemInnerSpacing);
        IMGUISTYLE_FROM_JSON(indentSpacing);
        IMGUISTYLE_FROM_JSON(cellPadding);
        IMGUISTYLE_FROM_JSON(scrollbarSize);
        IMGUISTYLE_FROM_JSON(scrollbarRounding);
        IMGUISTYLE_FROM_JSON(grabMinSize);
        IMGUISTYLE_FROM_JSON(grabRounding);
        IMGUISTYLE_FROM_JSON(tabRounding);
        IMGUISTYLE_FROM_JSON(buttonTextAlign);
        IMGUISTYLE_FROM_JSON(selectableTextAlign);
    }




    struct ImGuiColors {
        ImGuiPropVec4<ImGuiCol_Text> text;
        ImGuiPropVec4<ImGuiCol_TextDisabled> textDisabled;
        ImGuiPropVec4<ImGuiCol_WindowBg> windowBackground;
        ImGuiPropVec4<ImGuiCol_ChildBg> childBackground;
        ImGuiPropVec4<ImGuiCol_PopupBg> popupBackground;
        ImGuiPropVec4<ImGuiCol_Border> borderColor;
        ImGuiPropVec4<ImGuiCol_BorderShadow> borderShadow;
        ImGuiPropVec4<ImGuiCol_FrameBg> frameBackground;
        ImGuiPropVec4<ImGuiCol_FrameBgHovered> frameBackgroundHovered;
        ImGuiPropVec4<ImGuiCol_FrameBgActive> frameBackgroundActive;
        ImGuiPropVec4<ImGuiCol_TitleBg> titleBackground;
        ImGuiPropVec4<ImGuiCol_TitleBgActive> titleBackgroundActive;
        ImGuiPropVec4<ImGuiCol_TitleBgCollapsed> titleBackgroundCollapsed;
        ImGuiPropVec4<ImGuiCol_MenuBarBg> menubarBackground;
        ImGuiPropVec4<ImGuiCol_ScrollbarBg> scrollbarBackground;
        ImGuiPropVec4<ImGuiCol_ScrollbarGrab> scrollbarBackgroundGrab;
        ImGuiPropVec4<ImGuiCol_ScrollbarGrabHovered> scrollbarGrabHovered;
        ImGuiPropVec4<ImGuiCol_ScrollbarGrabActive> scrollbarGrabActive;
        ImGuiPropVec4<ImGuiCol_CheckMark> checkmark;
        ImGuiPropVec4<ImGuiCol_SliderGrab> sliderGrab;
        ImGuiPropVec4<ImGuiCol_SliderGrabActive> sliderGrabActive;
        ImGuiPropVec4<ImGuiCol_Button> button;
        ImGuiPropVec4<ImGuiCol_ButtonHovered> buttonHovered;
        ImGuiPropVec4<ImGuiCol_ButtonActive> buttonActive;
        ImGuiPropVec4<ImGuiCol_Header> header;
        ImGuiPropVec4<ImGuiCol_HeaderHovered> headerHovered;
        ImGuiPropVec4<ImGuiCol_HeaderActive> headerActive;
        ImGuiPropVec4<ImGuiCol_Separator> separator;
        ImGuiPropVec4<ImGuiCol_SeparatorHovered> separatorHovered;
        ImGuiPropVec4<ImGuiCol_SeparatorActive> separatorActive;
        ImGuiPropVec4<ImGuiCol_ResizeGrip> resizeGrip;
        ImGuiPropVec4<ImGuiCol_ResizeGripHovered> resizeGripHovered;
        ImGuiPropVec4<ImGuiCol_ResizeGripActive> resizeGripActive;
        ImGuiPropVec4<ImGuiCol_Tab> tab;
        ImGuiPropVec4<ImGuiCol_TabHovered> tabHovered;
        ImGuiPropVec4<ImGuiCol_TabActive> tabActive;
        ImGuiPropVec4<ImGuiCol_TabUnfocused> tabUnfocused;
        ImGuiPropVec4<ImGuiCol_TabUnfocusedActive> tabUnfocusedActive;
        ImGuiPropVec4<ImGuiCol_PlotLines> plotLines;
        ImGuiPropVec4<ImGuiCol_PlotLinesHovered> plotLinesHovered;
        ImGuiPropVec4<ImGuiCol_PlotHistogram> plotHistogram;
        ImGuiPropVec4<ImGuiCol_PlotHistogramHovered> plotHistogramHovered;
        ImGuiPropVec4<ImGuiCol_TableHeaderBg> TableHeaderBackground;
        ImGuiPropVec4<ImGuiCol_TableBorderStrong> tableBorderStrong;
        ImGuiPropVec4<ImGuiCol_TableBorderLight> tableBorderLight;
        ImGuiPropVec4<ImGuiCol_TableRowBg> tableRowBackground;
        ImGuiPropVec4<ImGuiCol_TableRowBgAlt> tableRowBackgroundAlt;
        ImGuiPropVec4<ImGuiCol_TextSelectedBg> textSelectedBackground;
        ImGuiPropVec4<ImGuiCol_DragDropTarget> dragDropTarget;
        ImGuiPropVec4<ImGuiCol_NavHighlight> navHighlight;
        ImGuiPropVec4<ImGuiCol_NavWindowingHighlight> navWindowingHighlight;
        ImGuiPropVec4<ImGuiCol_NavWindowingDimBg> navWindowingDimBackground;
        ImGuiPropVec4<ImGuiCol_ModalWindowDimBg> modalWindowDimBackground;

        void push() {
            text.push();
            textDisabled.push();
            windowBackground.push();
            childBackground.push();
            popupBackground.push();
            borderColor.push();
            borderShadow.push();
            frameBackground.push();
            frameBackgroundHovered.push();
            frameBackgroundActive.push();
            titleBackground.push();
            titleBackgroundActive.push();
            titleBackgroundCollapsed.push();
            menubarBackground.push();
            scrollbarBackground.push();
            scrollbarBackgroundGrab.push();
            scrollbarGrabHovered.push();
            scrollbarGrabActive.push();
            checkmark.push();
            sliderGrab.push();
            sliderGrabActive.push();
            button.push();
            buttonHovered.push();
            buttonActive.push();
            header.push();
            headerHovered.push();
            headerActive.push();
            separator.push();
            separatorHovered.push();
            separatorActive.push();
            resizeGrip.push();
            resizeGripHovered.push();
            resizeGripActive.push();
            tab.push();
            tabHovered.push();
            tabActive.push();
            tabUnfocused.push();
            tabUnfocusedActive.push();
            plotLines.push();
            plotLinesHovered.push();
            plotHistogram.push();
            plotHistogramHovered.push();
            TableHeaderBackground.push();
            tableBorderStrong.push();
            tableBorderLight.push();
            tableRowBackground.push();
            tableRowBackgroundAlt.push();
            textSelectedBackground.push();
            dragDropTarget.push();
            navHighlight.push();
            navWindowingHighlight.push();
            navWindowingDimBackground.push();
            modalWindowDimBackground.push();
        }

        void pop() {
            text.pop();
            textDisabled.pop();
            windowBackground.pop();
            childBackground.pop();
            popupBackground.pop();
            borderColor.pop();
            borderShadow.pop();
            frameBackground.pop();
            frameBackgroundHovered.pop();
            frameBackgroundActive.pop();
            titleBackground.pop();
            titleBackgroundActive.pop();
            titleBackgroundCollapsed.pop();
            menubarBackground.pop();
            scrollbarBackground.pop();
            scrollbarBackgroundGrab.pop();
            scrollbarGrabHovered.pop();
            scrollbarGrabActive.pop();
            checkmark.pop();
            sliderGrab.pop();
            sliderGrabActive.pop();
            button.pop();
            buttonHovered.pop();
            buttonActive.pop();
            header.pop();
            headerHovered.pop();
            headerActive.pop();
            separator.pop();
            separatorHovered.pop();
            separatorActive.pop();
            resizeGrip.pop();
            resizeGripHovered.pop();
            resizeGripActive.pop();
            tab.pop();
            tabHovered.pop();
            tabActive.pop();
            tabUnfocused.pop();
            tabUnfocusedActive.pop();
            plotLines.pop();
            plotLinesHovered.pop();
            plotHistogram.pop();
            plotHistogramHovered.pop();
            TableHeaderBackground.pop();
            tableBorderStrong.pop();
            tableBorderLight.pop();
            tableRowBackground.pop();
            tableRowBackgroundAlt.pop();
            textSelectedBackground.pop();
            dragDropTarget.pop();
            navHighlight.pop();
            navWindowingHighlight.pop();
            navWindowingDimBackground.pop();
            modalWindowDimBackground.pop();
        }
    };

    // JSON serialization
    inline void to_json(nlohmann::json& j, const ImGuiColors& p) {
        IMGUISTYLE_TO_JSON(text);
        IMGUISTYLE_TO_JSON(textDisabled);
        IMGUISTYLE_TO_JSON(windowBackground);
        IMGUISTYLE_TO_JSON(childBackground);
        IMGUISTYLE_TO_JSON(popupBackground);
        IMGUISTYLE_TO_JSON(borderColor);
        IMGUISTYLE_TO_JSON(borderShadow);
        IMGUISTYLE_TO_JSON(frameBackground);
        IMGUISTYLE_TO_JSON(frameBackgroundHovered);
        IMGUISTYLE_TO_JSON(frameBackgroundActive);
        IMGUISTYLE_TO_JSON(titleBackground);
        IMGUISTYLE_TO_JSON(titleBackgroundActive);
        IMGUISTYLE_TO_JSON(titleBackgroundCollapsed);
        IMGUISTYLE_TO_JSON(menubarBackground);
        IMGUISTYLE_TO_JSON(scrollbarBackground);
        IMGUISTYLE_TO_JSON(scrollbarBackgroundGrab);
        IMGUISTYLE_TO_JSON(scrollbarGrabHovered);
        IMGUISTYLE_TO_JSON(scrollbarGrabActive);
        IMGUISTYLE_TO_JSON(checkmark);
        IMGUISTYLE_TO_JSON(sliderGrab);
        IMGUISTYLE_TO_JSON(sliderGrabActive);
        IMGUISTYLE_TO_JSON(button);
        IMGUISTYLE_TO_JSON(buttonHovered);
        IMGUISTYLE_TO_JSON(buttonActive);
        IMGUISTYLE_TO_JSON(header);
        IMGUISTYLE_TO_JSON(headerHovered);
        IMGUISTYLE_TO_JSON(headerActive);
        IMGUISTYLE_TO_JSON(separator);
        IMGUISTYLE_TO_JSON(separatorHovered);
        IMGUISTYLE_TO_JSON(separatorActive);
        IMGUISTYLE_TO_JSON(resizeGrip);
        IMGUISTYLE_TO_JSON(resizeGripHovered);
        IMGUISTYLE_TO_JSON(resizeGripActive);
        IMGUISTYLE_TO_JSON(tab);
        IMGUISTYLE_TO_JSON(tabHovered);
        IMGUISTYLE_TO_JSON(tabActive);
        IMGUISTYLE_TO_JSON(tabUnfocused);
        IMGUISTYLE_TO_JSON(tabUnfocusedActive);
        IMGUISTYLE_TO_JSON(plotLines);
        IMGUISTYLE_TO_JSON(plotLinesHovered);
        IMGUISTYLE_TO_JSON(plotHistogram);
        IMGUISTYLE_TO_JSON(plotHistogramHovered);
        IMGUISTYLE_TO_JSON(TableHeaderBackground);
        IMGUISTYLE_TO_JSON(tableBorderStrong);
        IMGUISTYLE_TO_JSON(tableBorderLight);
        IMGUISTYLE_TO_JSON(tableRowBackground);
        IMGUISTYLE_TO_JSON(tableRowBackgroundAlt);
        IMGUISTYLE_TO_JSON(textSelectedBackground);
        IMGUISTYLE_TO_JSON(dragDropTarget);
        IMGUISTYLE_TO_JSON(navHighlight);
        IMGUISTYLE_TO_JSON(navWindowingHighlight);
        IMGUISTYLE_TO_JSON(navWindowingDimBackground);
        IMGUISTYLE_TO_JSON(modalWindowDimBackground);
    }

    inline void from_json(const nlohmann::json& j, ImGuiColors& p) {
        IMGUISTYLE_FROM_JSON(text);
        IMGUISTYLE_FROM_JSON(textDisabled);
        IMGUISTYLE_FROM_JSON(windowBackground);
        IMGUISTYLE_FROM_JSON(childBackground);
        IMGUISTYLE_FROM_JSON(popupBackground);
        IMGUISTYLE_FROM_JSON(borderColor);
        IMGUISTYLE_FROM_JSON(borderShadow);
        IMGUISTYLE_FROM_JSON(frameBackground);
        IMGUISTYLE_FROM_JSON(frameBackgroundHovered);
        IMGUISTYLE_FROM_JSON(frameBackgroundActive);
        IMGUISTYLE_FROM_JSON(titleBackground);
        IMGUISTYLE_FROM_JSON(titleBackgroundActive);
        IMGUISTYLE_FROM_JSON(titleBackgroundCollapsed);
        IMGUISTYLE_FROM_JSON(menubarBackground);
        IMGUISTYLE_FROM_JSON(scrollbarBackground);
        IMGUISTYLE_FROM_JSON(scrollbarBackgroundGrab);
        IMGUISTYLE_FROM_JSON(scrollbarGrabHovered);
        IMGUISTYLE_FROM_JSON(scrollbarGrabActive);
        IMGUISTYLE_FROM_JSON(checkmark);
        IMGUISTYLE_FROM_JSON(sliderGrab);
        IMGUISTYLE_FROM_JSON(sliderGrabActive);
        IMGUISTYLE_FROM_JSON(button);
        IMGUISTYLE_FROM_JSON(buttonHovered);
        IMGUISTYLE_FROM_JSON(buttonActive);
        IMGUISTYLE_FROM_JSON(header);
        IMGUISTYLE_FROM_JSON(headerHovered);
        IMGUISTYLE_FROM_JSON(headerActive);
        IMGUISTYLE_FROM_JSON(separator);
        IMGUISTYLE_FROM_JSON(separatorHovered);
        IMGUISTYLE_FROM_JSON(separatorActive);
        IMGUISTYLE_FROM_JSON(resizeGrip);
        IMGUISTYLE_FROM_JSON(resizeGripHovered);
        IMGUISTYLE_FROM_JSON(resizeGripActive);
        IMGUISTYLE_FROM_JSON(tab);
        IMGUISTYLE_FROM_JSON(tabHovered);
        IMGUISTYLE_FROM_JSON(tabActive);
        IMGUISTYLE_FROM_JSON(tabUnfocused);
        IMGUISTYLE_FROM_JSON(tabUnfocusedActive);
        IMGUISTYLE_FROM_JSON(plotLines);
        IMGUISTYLE_FROM_JSON(plotLinesHovered);
        IMGUISTYLE_FROM_JSON(plotHistogram);
        IMGUISTYLE_FROM_JSON(plotHistogramHovered);
        IMGUISTYLE_FROM_JSON(TableHeaderBackground);
        IMGUISTYLE_FROM_JSON(tableBorderStrong);
        IMGUISTYLE_FROM_JSON(tableBorderLight);
        IMGUISTYLE_FROM_JSON(tableRowBackground);
        IMGUISTYLE_FROM_JSON(tableRowBackgroundAlt);
        IMGUISTYLE_FROM_JSON(textSelectedBackground);
        IMGUISTYLE_FROM_JSON(dragDropTarget);
        IMGUISTYLE_FROM_JSON(navHighlight);
        IMGUISTYLE_FROM_JSON(navWindowingHighlight);
        IMGUISTYLE_FROM_JSON(navWindowingDimBackground);
        IMGUISTYLE_FROM_JSON(modalWindowDimBackground);
    }

    enum class RoundingType {
        NONE,					// Hard corners
        ROUNDED,				// Round corners
        HALF_ROUND,				// Radius is half the button width
    };

    class Rounding {
    public:
        Rounding() = default;
        Rounding(RoundingType type, float amount) {
            this->type = type;
            this->amount = amount;
        }

        void push() {
            if (type == RoundingType::ROUNDED)
                amount.push();
            if (type == RoundingType::HALF_ROUND)
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 99999);
        }

        void pop() {
            if (type == RoundingType::ROUNDED)
                amount.pop();
            if (type == RoundingType::HALF_ROUND)
                ImGui::PopStyleVar();
        }

        ImGuiPropFloat<ImGuiStyleVar_FrameRounding> amount;
        RoundingType type = RoundingType::NONE;
    };

    inline void to_json(nlohmann::json& j, const Rounding& p) {
        j["amount"] = p.amount;
        j["type"] = magic_enum::enum_name(p.type);
    }
    inline void from_json(const nlohmann::json& j, Rounding& p) {
        j["amount"].get_to(p.amount);
        auto opt = magic_enum::enum_cast<RoundingType>((std::string)j["type"]);
        if (opt.has_value()) {
            p.type = opt.value();
        }
    }
/*
    class SameLine {
    public:
        SameLine() = default;

        SameLine& operator=(bool sameline) {
            this->sameline = sameline;
            return *this;
        }

        operator bool() {
            return sameline;
        }

        void operator()() {
            if (sameline) ImGui::SameLine();
        }

        friend void to_json(nlohmann::json& j, const SameLine& p) {
            j = p.sameline;
        }
        friend void from_json(const nlohmann::json& j, SameLine& p) {
            j.get_to(p.sameline);
        }

    private:
        bool sameline = false;
    };*/

    class Measurement {
    public:

        enum class Unit {
            NONE,
            UNITLESS,
            PIXEL,
            PERCENT
        };

        Measurement() {}
        Measurement(const char* value) {
            this->operator=(value);
        }
        Measurement(const std::string& value) {
            this->operator=(value);
        }
        Measurement(float value, Unit unit) {
            this->value_ = value;
            this->unit_ = unit;
        }

        // This function takes a value_ string such as '0' or '47px' or '80%'
        Measurement& operator=(const char* value) {
            this->operator=(std::string(value));
            return *this;
        }
        Measurement& operator=(const std::string& value) {
            float tempValue = 0.f;
            Unit tempUnit = Unit::NONE;

            if (value.empty())                                              // Is empty
                throw UI_EXCEPTION("Cannot load value: Value string is empty!");

            if (std::string("0123456789.").find(value[0]) == std::string::npos)       // Doesn't start with a digit
                throw UI_EXCEPTION("Cannot load value '%s': Expected a digit!", value.c_str());

            size_t numberDigits = value.find_first_not_of("0123456789.");
            if (numberDigits == std::string::npos) numberDigits = value.length();            // It must consist only of digits

            try {
                tempValue = std::stof(value.substr(0, numberDigits));
            }
            catch (...) {
                throw UI_EXCEPTION("Cannot load value '%s': Parsing as float failed", value.c_str());
            }

            bool digitsOnly = numberDigits == value.length();
            if (digitsOnly) {
                tempUnit = Unit::UNITLESS;
            }
            else {
                std::string unit = value.substr(numberDigits, value.length() - numberDigits);
                if (unit == "px") tempUnit = Unit::PIXEL;
                else if (unit == "%") tempUnit = Unit::PERCENT;
                else throw UI_EXCEPTION("Cannot load value '%s': Unit is not known!", value.c_str());
            }

            this->value_ = tempValue;
            this->unit_ = tempUnit;

            return *this;
        }

        float getValue() const {
            return value_;
        }

        Unit getUnit() const {
            return unit_;
        }

        bool operator==(const Measurement& other) {
            return (this->unit_ == other.unit_) && (this->value_ == other.value_);
        }

        bool operator!=(const Measurement& other) {
            return !this->operator==(other);
        }

    private:
        float value_ = 0.f;
        Unit unit_ = Unit::NONE;
    };

}
