cmake_minimum_required(VERSION 3.16)

project(BatteryUI VERSION 0.1.1 LANGUAGES CXX)

add_library(BatteryUI INTERFACE)
add_library(BatteryUI::BatteryUI ALIAS BatteryUI)

target_compile_features(BatteryUI INTERFACE cxx_std_17)

target_include_directories(BatteryUI INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_sources(BatteryUI INTERFACE)
