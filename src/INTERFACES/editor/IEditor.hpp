#ifndef _OPENMAP_IEDITOR_HPP
#define _OPENMAP_IEDITOR_HPP

#include <string>
#include <vector>
#include <src/INTERFACES/core/IDraw.hpp>
#include <src/INTERFACES/core/IInput.hpp>


class IEditor {

public:
        enum class MapObjectType {
                MODEL,
                SPRITE
        };


        virtual int Run(
                IDraw* drawer,
                IInput* input
        ) = 0;

        virtual void AddMapObject(
                MapObjectType type,
                std::string path,
                std::vector<std::string> extra_data = {}
        ) = 0;

        virtual ~IEditor() = default;

};

#endif // _OPENMAP_IEDITOR_HPP