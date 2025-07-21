#ifndef _OPENMAP_IRENDERER_HPP
#define _OPENMAP_IRENDERER_HPP

#include <string>
#include <vector>
#include <src/INTERFACES/editor/IEditor.hpp>
#include <src/INTERFACES/core/IWindower.hpp>


class IRenderer {

public:
        struct Model; // opaque pointer


        // Loads 3D models *and* images as sprites (textures mapped onto planes)
        virtual Model* Load(const std::string path) = 0;

        virtual void Render(
                const std::vector<IEditor::MapObjectInstance>& map_object_instances,
                const std::vector<IEditor::MapObjectInstance*>& selected_map_objects,
                const IWindower::InputState& input_state
        ) = 0;

        virtual ~IRenderer() = default;

};

#endif // _OPENMAP_IRENDERER_HPP