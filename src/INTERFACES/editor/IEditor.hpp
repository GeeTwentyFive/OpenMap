#ifndef _OPENMAP_IEDITOR_HPP
#define _OPENMAP_IEDITOR_HPP

#include <string>
#include <array>
#include <src/INTERFACES/core/IRenderer.hpp>


class IEditor {

public:
        struct MapObjectInstance{
                std::string name;
                const IRenderer::Model* model;
                std::array<float, 3> pos;
                std::array<float, 3> rot;
                std::array<float, 3> scale;
                std::string extra_data = {};
                bool selected = false;
        };


        virtual void RegisterMapObject(
                const std::string& path,
                const std::string& default_extra_data = {}
        ) = 0;

        virtual void Run() = 0;
        
        virtual void AddMapObject(
                const std::string& name, // <= filename in `path` in `RegisterMapObject()` (without extension)
                const std::array<float, 3>& pos,
                const std::array<float, 3>& rot,
                const std::array<float, 3>& scale,
                const std::string& extra_data = {}
        ) = 0;

        virtual void Save(const std::string& path) = 0;
        virtual void Load(const std::string& path) = 0;
        virtual void Clear() = 0;

        virtual ~IEditor() = default;

};

#endif // _OPENMAP_IEDITOR_HPP