#include <src/IMPLEMENTATIONS/editor/IEditor/Editor.hpp>

#include <stdexcept>
#include <iostream>
#include <src/IMPLEMENTATIONS/core/IDraw/RaylibDrawer.hpp>
#include <src/IMPLEMENTATIONS/core/IInput/RaylibInput.hpp>
#include <src/IMPLEMENTATIONS/core/IGUI/RayguiGUI.hpp>


#define LOG_FILE_NAME "OpenMap.log"
#define OPENMAP_CONFIG_SCRIPT "config.chai"


int main() {

        try {
                freopen(LOG_FILE_NAME, "w", stdout);
                freopen(LOG_FILE_NAME, "w", stderr);

                return Editor().Run(
                        new RaylibDrawer(
                                (ConfigFlags)(
                                        FLAG_WINDOW_RESIZABLE |
                                        FLAG_WINDOW_MAXIMIZED |
                                        FLAG_MSAA_4X_HINT
                                )
                        ),
                        new RaylibInput(),
                        new RayguiGUI(),
                        OPENMAP_CONFIG_SCRIPT
                );
        } catch(const std::exception& e) {
                std::cout << e.what() << std::endl;
        }

        return -1;

}
