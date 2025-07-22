#include <src/IMPLEMENTATIONS/editor/IEditor/Editor.hpp>

#include <stdio.h>
#include <stdexcept>
#include <iostream>
#include <src/IMPLEMENTATIONS/core/IWindower/GLFWWindower.hpp>
#include <src/IMPLEMENTATIONS/core/IRenderer/OpenGLRenderer.hpp>
#include <src/IMPLEMENTATIONS/core/IGUI/DearImGuiGUI.hpp>
#include <src/IMPLEMENTATIONS/core/ISerializer/JSONSerializer.hpp>


#define LOG_FILE_NAME "OpenMap.log"
#define CONFIG_FILE_NAME "config"


int main() {

        freopen(LOG_FILE_NAME, "w", stdout);
        freopen(LOG_FILE_NAME, "w", stderr);

        try {
                Editor(
                        new GLFWWindower(),
                        new OpenGLRenderer(),
                        new DearImGuiGUI(),
                        new JSONSerializer()
                ).Run();
        } catch(const std::exception& e) {
                std::cout << e.what() << std::endl;
                return -1;
        }

        return 0;

}
