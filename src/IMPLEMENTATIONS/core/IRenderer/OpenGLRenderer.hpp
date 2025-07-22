#ifndef _OPENMAP_OPENGLRENDERER_HPP
#define _OPENMAP_OPENGLRENDERER_HPP

#include <src/INTERFACES/core/IRenderer.hpp>

#include <src/INTERFACES/editor/IEditor.hpp>


class OpenGLRenderer : public IRenderer {

private:
        // TODO:
        // - Draw(map_objects)
        // - DrawBoundingBoxes(selected_map_objects)


public:
        OpenGLRenderer() {
                // TODO
        }

        inline Model* Load(const std::string& path) override {
                // TODO
        }

        inline void Render(
                const std::vector<IEditor::MapObjectInstance>& map_object_instances,
                const std::vector<IEditor::MapObjectInstance*>& selected_map_objects
        ) override {
                // TODO
        }

};

#endif // _OPENMAP_OPENGLRENDERER_HPP