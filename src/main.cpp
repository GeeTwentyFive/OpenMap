#include <src/IMPLEMENTATIONS/editor/IEditor/Editor.hpp>

#include <stdexcept>
#include <iostream>
#include <src/IMPLEMENTATIONS/core/IDraw/RaylibDrawer.hpp>
#include <src/IMPLEMENTATIONS/core/IInput/RaylibInput.hpp>


#define OPENMAP_CONFIG_SCRIPT "config.chai"
#define OPENMAP_EXPORT_SCRIPT "export.chai"


int main() {

        try {
                return Editor().Run(
                        new RaylibDrawer(
                                (ConfigFlags)(
                                        FLAG_WINDOW_RESIZABLE |
                                        FLAG_WINDOW_MAXIMIZED |
                                        FLAG_MSAA_4X_HINT
                                )
                        ),
                        new RaylibInput(),
                        OPENMAP_CONFIG_SCRIPT,
                        OPENMAP_EXPORT_SCRIPT
                );
        } catch(const std::exception& e) {
                std::cout << e.what() << std::endl;
        }

        return -1;

}
