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

        virtual void UpdateCamera(
                std::array<float, 3> movement,
                std::array<float, 2> rotation
        ) = 0;

        virtual void ClearFramebuffer() = 0;

        virtual void Render(
                const std::vector<MapObjectInstance>& map_object_instances,
                const std::vector<size_t>& selected_map_objects_indices
        ) = 0;

        virtual ~IRenderer() = default;

};

#endif // _OPENMAP_IRENDERER_HPP