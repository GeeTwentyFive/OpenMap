#ifndef _OPENMAP_IDRAW_HPP
#define _OPENMAP_IDRAW_HPP

#include <array>
#include <cstdint>

class IDraw {

public:
        typedef struct Model;

        struct BoundingBox {
                std::array<float, 3> min;
                std::array<float, 3> max;
        };

        struct Ray {
                std::array<float, 3> pos;
                std::array<float, 3> direction;
        };

        struct RayCollision {
                bool hit;
                float distance_to_hit;
                std::array<float, 3> hit_point;
                std::array<float, 3> hit_normal;
        };


        virtual Model* LoadModel(const char* target_file_path) = 0;
        virtual Model* LoadSprite(const char* target_file_path) = 0;
        virtual void Unload(Model* model) = 0;

        virtual bool WindowShouldClose() = 0;

        virtual void UpdateCamera(std::array<float, 3> movement, std::array<float, 2> rotation) = 0;

        virtual std::array<float, 3> GetCameraPosition() = 0;

        virtual void BeginDrawing(int32_t clear_color = 0x000000ff) = 0;

        virtual void Draw(
                Model* model,
                std::array<float, 3> position,
                std::array<float, 3> rotation,
                std::array<float, 3> scale,
                int32_t color_tint = 0xffffffff
        ) = 0;

        virtual BoundingBox GetModelBoundingBox(Model* model) = 0;
        virtual void DrawBoundingBox(BoundingBox bounding_box, int32_t color = 0xffffffff) = 0;

        virtual Ray GetScreenToWorldRay(std::tuple<int, int> pos) = 0;

        virtual RayCollision GetRayCollisionBox(Ray ray, BoundingBox box) = 0;

        virtual void EndDrawing() = 0;

        virtual ~IDraw() = default;

};

#endif // _OPENMAP_IDRAW_HPP