#pragma once

// This is the central place for adding new widgets, these macros are expanded in the right locations
// This is done so only this file needs to be changed when adding a widget

#define BATTERYUI_WIDGET_DEFAULT_STYLE_DECL() \
    inline ButtonStyle* GetButtonDefaultStyle(); \
    inline DropdownStyle* GetDropdownDefaultStyle(); \
    inline ContainerStyle* GetChildDefaultStyle(); \
    inline HorizontalGridStyle* GetHorizontalGridDefaultStyle(); \
    inline VerticalGridStyle* GetVerticalGridDefaultStyle();

#define BATTERYUI_WIDGET_DEFAULT_STYLE_IMPL() \
    inline ButtonStyle* GetButtonDefaultStyle() { return &RootUI::defaultStyle.button; } \
    inline DropdownStyle* GetDropdownDefaultStyle() { return &RootUI::defaultStyle.dropdown; } \
    inline ContainerStyle* GetChildDefaultStyle() { return &RootUI::defaultStyle.container; } \
    inline HorizontalGridStyle* GetHorizontalGridDefaultStyle() { return &RootUI::defaultStyle.horizontalGrid; } \
    inline VerticalGridStyle* GetVerticalGridDefaultStyle() { return &RootUI::defaultStyle.verticalGrid; }

#define BATTERYUI_WIDGET_DEFAULT_STYLE_DEFINITION() \
struct DefaultStyles { \
 \
    ImGuiStyle imguiStyle; \
    ImGuiColors imguiColors; \
 \
    ButtonStyle button; \
    DropdownStyle dropdown; \
    ContainerStyle container; \
    HorizontalGridStyle horizontalGrid; \
    VerticalGridStyle verticalGrid; \
 \
    DefaultStyles() { \
        imguiStyle = ImGuiStyle(); \
        imguiColors = ImGuiColors(); \
 \
        Button::Presets::load(); \
        button = Button::Presets::Modern; \
 \
        Dropdown::Presets::load(); \
        dropdown = Dropdown::Presets::Modern; \
 \
        Container::Presets::load(); \
        container = Container::Presets::Thin; \
 \
        HorizontalGrid::Presets::load(); \
        horizontalGrid = HorizontalGrid::Presets::Hidden; \
 \
        VerticalGrid::Presets::load(); \
        verticalGrid = VerticalGrid::Presets::Hidden; \
    } \
 \
    void push() { \
        imguiStyle.push(); \
        imguiColors.push(); \
    } \
 \
    void pop() { \
        imguiColors.pop(); \
        imguiStyle.pop(); \
    } \
};
