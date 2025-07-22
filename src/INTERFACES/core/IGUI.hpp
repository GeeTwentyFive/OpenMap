#ifndef _OPENMAP_IGUI_HPP
#define _OPENMAP_IGUI_HPP

#include <utility>
#include <vector>
#include <string>
#include <functional>
#include <src/INTERFACES/editor/IEditor.hpp>
#include <src/INTERFACES/core/IRenderer.hpp>
#include <src/INTERFACES/core/IWindower.hpp>


class IGUI {

public:
        virtual void Update(
                const std::pair<int, int>& window_resolution,
                const InputState& input_state,
                const std::vector<MapObjectRegistration>& registered_map_objects,
                const std::vector<MapObjectInstance>& map_object_instances,
                const std::vector<MapObjectInstance*>& selected_map_objects,

                const std::function<void(std::string)>& save_callback,
                const std::function<void(std::string)>& load_callback,
                const std::function<void()>& clear_callback,
                const std::function<void(std::string)>& add_map_object_callback
        ) = 0;

        virtual ~IGUI() = default;

};

#endif // _OPENMAP_IGUI_HPP