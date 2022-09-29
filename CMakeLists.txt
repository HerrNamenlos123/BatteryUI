cmake_minimum_required(VERSION 3.16)

project(BatteryUI VERSION 0.1.1 LANGUAGES CXX)

add_library(BatteryUI INTERFACE)
add_library(BatteryUI::BatteryUI ALIAS BatteryUI)

target_compile_features(BatteryUI INTERFACE cxx_std_17)

target_include_directories(BatteryUI INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

set(HEADER_FILES
        include/BatteryUI/Widgets/Button.h
        include/BatteryUI/Widgets/Dropdown.h
        include/BatteryUI/Widgets/BasicWidget.h
        include/BatteryUI/BatteryUI.h
        include/BatteryUI/common.h
        include/BatteryUI/exception.h
        include/BatteryUI/FileWatcher.h
        include/BatteryUI/Font.h
        include/BatteryUI/InternalDecl.h
        include/BatteryUI/InternalImpl.h
        include/BatteryUI/json.hpp
        include/BatteryUI/magic_enum.h
        include/BatteryUI/Property.h
        include/BatteryUI/RedrawNotifier.h
        include/BatteryUI/RootUI.h
        include/BatteryUI/serialize.h
        include/BatteryUI/Style.h
        include/BatteryUI/Window.h)

target_sources(BatteryUI INTERFACE ${HEADER_FILES})
