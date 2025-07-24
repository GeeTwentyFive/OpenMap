cmake_minimum_required(VERSION 3.15)

project(OpenMap)


add_executable(
        OpenMap
        ./src/main.cpp
        ./libs/glad/glad.c
)

target_include_directories(
        OpenMap
        PUBLIC .
        PUBLIC ./libs/tinygltf
        PUBLIC ./libs/glad
        PUBLIC ./libs/glm
        PUBLIC ./libs/imgui
        PUBLIC ./libs/ImGuiFileDialog
        PUBLIC ./libs/ImGuizmo
)


if(WIN32)

        target_link_libraries(
                OpenMap
                ${PROJECT_SOURCE_DIR}/libs/glfw/libs/libglfw3_windows.a
        )

elseif(UNIX)

        target_link_libraries(
                OpenMap
                ${PROJECT_SOURCE_DIR}/libs/glfw/libs/libglfw3_linux.a
        )

endif()