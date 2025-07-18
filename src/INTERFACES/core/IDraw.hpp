#ifndef _OPENMAP_IDRAW_HPP
#define _OPENMAP_IDRAW_HPP

#include <array>

class IDraw {

public:
        typedef struct Model;


        virtual Model* LoadModel(const char* target_file_path) = 0;
        virtual Model* LoadSprite(const char* target_file_path) = 0;
        virtual void Unload(Model* model) = 0;


        virtual bool WindowShouldClose() = 0;

        virtual void UpdateCamera(float movement[3], float rotation[2]) = 0;

        virtual std::array<float, 3> GetCameraPosition() = 0;

        virtual void BeginDrawing(int clear_color = 0x000000ff) = 0;

        virtual void Draw(
                Model* model,
                float position[3],
                float rotation[3],
                float scale[3],
                int color_tint = 0xffffffff
        ) = 0;

        virtual void EndDrawing() = 0;

        virtual ~IDraw() = default;

};

#endif // _OPENMAP_IDRAW_HPP