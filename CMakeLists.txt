cmake_minimum_required(VERSION 3.15)

project(OpenMap)


add_executable(
        OpenMap
        src/main.cpp
        libs/raylib-gizmo/raygizmo.c
)

target_include_directories(
        OpenMap
        PUBLIC .
        PUBLIC libs/raylib/include
        PUBLIC libs/raylib-gizmo
        PUBLIC libs/chaiscript
)


if(WIN32)

        target_compile_definitions(
                OpenMap
                PUBLIC NOGDI
                PUBLIC NOUSER
        )

        target_link_libraries(
                OpenMap
                ${PROJECT_SOURCE_DIR}/libs/raylib/lib/libraylib_windows.a
                winmm
        )

elseif(APPLE)

        target_link_libraries(
                OpenMap
		${PROJECT_SOURCE_DIR}/libs/raylib/lib/libraylib_macos.a
                "-framework IOKit"
                "-framework Cocoa"
                "-framework OpenGL"
        )

elseif(UNIX)

        target_link_libraries(
                OpenMap
		${PROJECT_SOURCE_DIR}/libs/raylib/lib/libraylib_linux.a
        )

endif()
