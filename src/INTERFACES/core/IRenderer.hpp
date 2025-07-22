#ifndef _OPENMAP_IRENDERER_HPP
#define _OPENMAP_IRENDERER_HPP

#include <string>
#include <vector>
struct MapObjectInstance;
#include <src/INTERFACES/editor/IEditor.hpp>


struct Model; // opaque pointer


class IRenderer {

public:
        virtual Model* Load(const std::string& path) = 0;

        // TODO: UpdateCamera(...?)

        virtual void Render(
                const std::vector<MapObjectInstance>& map_object_instances,
                const std::vector<MapObjectInstance*>& selected_map_objects
        ) = 0;

        virtual ~IRenderer() = default;

};

#endif // _OPENMAP_IRENDERER_HPP