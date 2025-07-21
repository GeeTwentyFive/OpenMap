#ifndef _OPENMAP_IRENDERER_HPP
#define _OPENMAP_IRENDERER_HPP

#include <string>
#include <array>


class IRenderer { // TODO

public:
        struct Model; // opaque pointer


        virtual Model* Load(const std::string path) = 0;

        virtual void Draw(
                const Model* model,
                const std::array<float, 3>& position,
                const std::array<float, 3>& rotation,
                const std::array<float, 3>& scale,
                const std::array<unsigned char, 4>& color_tint = {0xff, 0xff, 0xff, 0xff}
        ) = 0;

        virtual ~IRenderer() = default;

};

#endif // _OPENMAP_IRENDERER_HPP