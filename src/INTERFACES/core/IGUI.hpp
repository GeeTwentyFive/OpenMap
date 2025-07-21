#ifndef _OPENMAP_IGUI_HPP
#define _OPENMAP_IGUI_HPP

#include <utility>
#include <vector>
#include <src/INTERFACES/core/IRenderer.hpp>
#include <src/INTERFACES/core/IWindower.hpp>
#include <src/INTERFACES/editor/IEditor.hpp>


class IGUI {

public:
        enum class ButtonPressed {
                NONE,
                SAVE,
                LOAD,
                EXPORT,
                CLEAR,
                MAP_OBJECT
        };
        virtual ButtonPressed Update(
                const std::pair<int, int>& window_resolution,
                const IWindower::InputState& input_state,
                const std::vector< std::pair<std::string, IRenderer::Model*> >& map_objects,
                const std::vector<IEditor::MapObjectInstance>& map_object_instances
        ) = 0;

        virtual ~IGUI() = default;

};

#endif // _OPENMAP_IGUI_HPP