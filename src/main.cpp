#include <src/IMPLEMENTATIONS/editor/IEditor/Editor.hpp>

#include <src/IMPLEMENTATIONS/core/IDraw/RaylibDrawer.hpp>


int main() {

        return Editor().Run(
                RaylibDrawer()
        );

}
