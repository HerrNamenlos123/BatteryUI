cmake_minimum_required(VERSION 3.16)

project(BatteryUI VERSION 0.1.1 LANGUAGES CXX)

add_library(BatteryUI INTERFACE)
add_library(BatteryUI::BatteryUI ALIAS BatteryUI)

target_include_directories(BatteryUI INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)
