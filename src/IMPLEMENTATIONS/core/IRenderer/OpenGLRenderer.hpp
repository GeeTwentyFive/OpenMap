#ifndef _OPENMAP_OPENGLRENDERER_HPP
#define _OPENMAP_OPENGLRENDERER_HPP

#include <src/INTERFACES/core/IRenderer.hpp>

#include <src/INTERFACES/editor/IEditor.hpp>


class OpenGLRenderer : public IRenderer {

private:
        // TODO:
        // - Draw map objects
        // - Draw bounding boxes around selected map objects


public:
        inline Model* Load(const std::string& path) override {
                // TODO
        }

        inline void Clear() override {
                glClear(GL_COLOR_BUFFER_BIT);
        }

        inline void Render(
                const std::vector<MapObjectInstance>& map_object_instances,
                const std::vector<size_t>& selected_map_objects_indices
        ) override {
                // TODO
        }

};

#endif // _OPENMAP_OPENGLRENDERER_HPP