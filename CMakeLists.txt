cmake_minimum_required(VERSION 3.15)

project(OpenMap)


add_executable(
        OpenMap
        ./src/main.cpp
)

target_include_directories(
        OpenMap
        PUBLIC .
        PUBLIC ./libs/raylib/include
        PUBLIC ./libs/raylib-cpp/include
        PUBLIC ./libs/imgui
        PUBLIC ./libs/ImGuiFileDialog
        PUBLIC ./libs/ImGuizmo
)


if(WIN32)

        target_link_libraries(
                OpenMap
                ${PROJECT_SOURCE_DIR}/libs/raylib/lib/libraylib_windows.a
                winmm
                "-mwindows"
        )

elseif(UNIX)

        target_link_libraries(
                OpenMap
                ${PROJECT_SOURCE_DIR}/libs/raylib/lib/libraylib_linux.a
        )

endif()