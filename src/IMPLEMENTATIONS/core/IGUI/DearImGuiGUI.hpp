#ifndef _OPENMAP_DEARIMGUIGUI_HPP
#define _OPENMAP_DEARIMGUIGUI_HPP

#include <src/INTERFACES/core/IGUI.hpp>


class DearImGuiGUI : public IGUI {

public:
        DearImGuiGUI() {
                // TODO
        }

        inline void Update(
                const std::pair<int, int>& window_resolution,
                const InputState& input_state,
                const std::vector<MapObjectRegistration>& registered_map_objects,
                const std::vector<MapObjectInstance>& map_object_instances,
                const std::vector<MapObjectInstance*>& selected_map_objects,

                const std::function<void(std::string)>& save_callback,
                const std::function<void(std::string)>& load_callback,
                const std::function<void()>& clear_callback,
                const std::function<void(std::string)>& add_map_object_callback
        ) override {
                // TODO
        }

};

#endif // _OPENMAP_DEARIMGUIGUI_HPP