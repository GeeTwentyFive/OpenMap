#ifndef _OPENMAP_IGUI_HPP
#define _OPENMAP_IGUI_HPP

#include <utility>
#include <vector>
#include <string>
#include <functional>
#include <src/INTERFACES/core/IRenderer.hpp>
#include <src/INTERFACES/core/IWindower.hpp>
#include <src/INTERFACES/editor/IEditor.hpp>


class IGUI {

public:
        virtual void Update(
                const std::pair<int, int>& window_resolution,
                const IWindower::InputState& input_state,
                const std::vector< std::pair<std::string, IRenderer::Model*> >& map_objects,
                const std::vector<IEditor::MapObjectInstance>& map_object_instances,
                const std::function<void(std::string)>& save_callback,
                const std::function<void(std::string)>& load_callback,
                const std::function<void()>& clear_callback,
                const std::function<void(std::string)>& add_map_object_callback
        ) = 0;

        virtual ~IGUI() = default;

};

#endif // _OPENMAP_IGUI_HPP