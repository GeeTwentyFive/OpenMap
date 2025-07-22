#ifndef _OPENMAP_IEDITOR_HPP
#define _OPENMAP_IEDITOR_HPP

#include <string>
#include <array>
#include <src/INTERFACES/core/IRenderer.hpp>


struct MapObjectRegistration{
        std::string name;
        Model* model;
        std::string default_extra_data = {};
};

struct MapObjectInstance{
        std::string name;
        std::array<float, 3> pos;
        std::array<float, 3> rot;
        std::array<float, 3> scale;
        std::string extra_data = {};
};


class IEditor {

public:
        virtual void RegisterMapObject(
                const std::string& path,
                const std::string& default_extra_data = {}
        ) = 0;

        virtual void Run() = 0;
        
        virtual void InstantiateMapObject(
                const std::string& name, // <= filename in `path` in `RegisterMapObject()` (without extension)
                const std::array<float, 3>& pos,
                const std::array<float, 3>& rot,
                const std::array<float, 3>& scale,
                const std::string& extra_data = {}
        ) = 0;

        virtual void Save(const std::string& path) = 0;
        virtual void Load(const std::string& path) = 0;

        virtual ~IEditor() = default;

};

#endif // _OPENMAP_IEDITOR_HPP