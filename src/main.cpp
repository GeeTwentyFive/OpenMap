#include <src/IMPLEMENTATIONS/editor/IEditor/Editor.hpp>

#include <src/IMPLEMENTATIONS/core/IDraw/RaylibDrawer.hpp>
#include <src/IMPLEMENTATIONS/core/IInput/RaylibInput.hpp>


int main() {

        return Editor().Run(
                RaylibDrawer(
                        (ConfigFlags)(
                                FLAG_WINDOW_RESIZABLE |
                                FLAG_WINDOW_MAXIMIZED |
                                FLAG_MSAA_4X_HINT
                        )
                ),
                RaylibInput()
        );

}
