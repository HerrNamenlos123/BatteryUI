cmake_minimum_required(VERSION 3.16)
project(uiExample LANGUAGES CXX)


###################
# Helper function #
###################

function(GET_GITHUB_DEPENDENCY NAME REQUIRED_FILE REPOSITORY_URL)
    if (NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/${REQUIRED_FILE}")
        find_package(Git REQUIRED)
        if(GIT_FOUND)
            message(STATUS "${REQUIRED_FILE} not found, getting dependency '${NAME}'")
            file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/modules)
            execute_process(COMMAND ${GIT_EXECUTABLE} clone ${REPOSITORY_URL}
                            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/modules
                            RESULT_VARIABLE GIT_RESULT)
            if(NOT GIT_RESULT EQUAL "0")
                message(FATAL_ERROR "git clone ${REPOSITORY_URL} failed. Error: '${GIT_RESULT}'. Please check the dependencies")
            endif()
        else()
            message(FATAL_ERROR "Cannot get dependencies, git executable not found. Please install Git")
        endif()
    endif()
endfunction()


################
# Dependencies #
################

get_github_dependency(imgui "modules/imgui/imgui.cpp" "https://github.com/ocornut/imgui.git")

include(cmake/imgui.cmake)


#####################
# Target definition #
#####################

file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "include/*")
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*")

add_executable(uiExample ${SRC_FILES} ${HEADER_FILES})

target_compile_features(uiExample PRIVATE cxx_std_17)
set_target_properties(uiExample PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(uiExample PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

if(MSVC)
    set_property(TARGET uiExample PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    string(APPEND CMAKE_CXX_FLAGS " /Zc:__cplusplus /MP")
else()
    target_compile_options(uiExample PRIVATE -Wno-psabi)
endif()



############################
# Preprocessor definitions #
############################

if (WIN32)
    target_compile_definitions(uiExample PRIVATE
       WIN32_LEAN_AND_MEAN      # Prevents Windows.h from adding unnecessary includes
       NOMINMAX                 # Prevents Windows.h from defining min/max as macros 
       _CRT_SECURE_NO_WARNINGS  # Prevents MSVC warnings about deprecated functions
    )
endif()



###########
# Linking #
###########

target_link_libraries(uiExample imgui::imgui)



#######
# IDE #
#######

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/src" PREFIX "Source Files" FILES ${SRC_FILES})
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}/include" PREFIX "Header Files" FILES ${HEADER_FILES})
