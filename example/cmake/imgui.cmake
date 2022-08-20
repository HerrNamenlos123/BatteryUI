
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/modules/imgui)
add_library(imgui STATIC)
add_library(imgui::imgui ALIAS imgui)

target_sources(imgui PRIVATE
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui.cpp
)

target_include_directories(imgui PUBLIC
    $<BUILD_INTERFACE:${IMGUI_DIR}/>
    $<INSTALL_INTERFACE:/>
)
