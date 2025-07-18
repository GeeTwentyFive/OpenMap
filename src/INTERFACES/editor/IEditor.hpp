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
                IInput* input,
                const char* config_file_name,
                const char* export_file_name = 0
        ) = 0;

        virtual void AddMapObject(
                MapObjectType type,
                std::string path,
                std::vector<std::string> extra_data = {}
        ) = 0;

        virtual void Export(std::string path) = 0;

        virtual ~IEditor() = default;

};

#endif // _OPENMAP_IEDITOR_HPP