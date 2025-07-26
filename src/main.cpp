#include <src/IMPLEMENTATIONS/editor/Editor.hpp>

#include <stdio.h>
#include <stdexcept>
#include <iostream>
#include <src/IMPLEMENTATIONS/core/Windower.hpp>
#include <src/IMPLEMENTATIONS/core/Renderer.hpp>
#include <src/IMPLEMENTATIONS/core/GUI.hpp>
#include <src/IMPLEMENTATIONS/core/Serializer.hpp>


#define LOG_FILE_NAME "OpenMap.log"
#define CONFIG_FILE_NAME "config"


int main() {

        freopen(LOG_FILE_NAME, "w", stdout);
        freopen(LOG_FILE_NAME, "w", stderr);

        try {
                Editor(
                        new Windower(),
                        new Renderer(),
                        new GUI(),
                        new Serializer(),
                        CONFIG_FILE_NAME
                ).Run();
        } catch(const std::exception& e) {
                std::cout << e.what() << std::endl;
                return -1;
        }

        return 0;

}
