#ifndef _OPENMAP_RAYLIB_DRAWER_HPP
#define _OPENMAP_RAYLIB_DRAWER_HPP

#include <src/INTERFACES/core/IDraw.hpp>

namespace rl {
#include <libs/raylib/include/raylib.h>
namespace math {
#include <libs/raylib/include/raymath.h>
}
}


class RaylibDrawer: IDraw {

private:
        rl::Camera camera;

public:
        RaylibDrawer(
                rl::ConfigFlags window_config_flags = rl::FLAG_MSAA_4X_HINT,
                int window_min_width = 1280,
                int window_min_height = 720,
                float camera_fov = 90
        ) {

                rl::SetConfigFlags(window_config_flags);

                rl::InitWindow(
                        window_min_width,
                        window_min_height,
                        ""
                );

                rl::SetTargetFPS(
                        rl::GetMonitorRefreshRate(rl::GetCurrentMonitor()) * 2
                );

                rl::SetWindowMinSize(window_min_width, window_min_height);

                camera.position = rl::Vector3{0.0f, 0.0f, 0.0f};
                camera.target = rl::Vector3{0.0f, 0.0f, 1.0f};
                camera.up = rl::Vector3{0.0f, 1.0f, 0.0f};
                camera.fovy = camera_fov;
                camera.projection = rl::CAMERA_PERSPECTIVE;

        }

        ~RaylibDrawer() {
                rl::CloseWindow();
        }


        Model* LoadModel(const char* target_file_path) override {
                rl::Model* model = new rl::Model;
                *model = rl::LoadModel(target_file_path);
                return (Model*)model;
        }
        void UnloadModel(Model* model) override {
                rl::UnloadModel(*(rl::Model*)model);
                delete[] model;
        }

        Sprite* LoadSprite(const char* target_file_path) override {
                rl::Texture2D* sprite = new rl::Texture2D;
                *sprite = rl::LoadTexture(target_file_path);
                return (Sprite*)&rl::LoadTexture(target_file_path);
        }
        void UnloadSprite(Sprite* sprite) override {
                rl::UnloadTexture(*(rl::Texture2D*)sprite);
                delete[] sprite;
        }


        bool WindowShouldClose() override {
                return rl::WindowShouldClose();
        }

        void BeginDrawing() override {
                rl::UpdateCamera(&camera, rl::CAMERA_FREE);
                rl::BeginDrawing();
                rl::ClearBackground(rl::Color{0xff, 0xff, 0xff, 0xff});
                rl::BeginMode3D(camera);
        }

        void Draw(
                Model* model,
                float position[3],
                float rotation[3],
                float scale[3],
                int color_tint = 0xffffffff
        ) override {
                rl::Model _model = *(rl::Model*)model;

                // Have to manually rotate since rl::DrawModelEx() doesn't support multi-axis rotation...
                _model.transform = rl::math::MatrixMultiply(
                        _model.transform,

                        rl::math::QuaternionToMatrix(
                                rl::math::QuaternionFromEuler(
                                        rotation[0],
                                        rotation[1],
                                        rotation[2]
                                )
                        )
                );

                rl::DrawModelEx(
                        _model,
                        rl::Vector3{
                                position[0],
                                position[1],
                                position[2]
                        },
                        rl::Vector3{0}, 0,
                        rl::Vector3{
                                scale[0],
                                scale[1],
                                scale[2]
                        },
                        rl::Color{
                                (unsigned char)((color_tint & 0xff000000) >> 24),
                                (unsigned char)((color_tint & 0x00ff0000) >> 16),
                                (unsigned char)((color_tint & 0x0000ff00) >> 8),
                                (unsigned char)((color_tint & 0x000000ff))
                        }
                );
        }

        void Draw(
                Sprite* sprite,
                float position[3],
                float rotation[3],
                float scale[3],
                int color_tint = 0xffffffff
        ) override {
                rl::Texture2D _sprite = *(rl::Texture2D*)sprite;

                rl::Mesh mesh = rl::GenMeshPlane(
                        _sprite.width,
                        _sprite.height,
                        1, 1
                );

                rl::Material material = rl::LoadMaterialDefault();
                material.maps[rl::MATERIAL_MAP_DIFFUSE].texture = _sprite;

                rl::Matrix transform = rl::math::MatrixIdentity();
                transform = rl::math::MatrixMultiply(
                        transform,
                        rl::math::MatrixScale(
                                scale[0],
                                scale[1],
                                scale[2]
                        )
                );
                transform = rl::math::MatrixMultiply(
                        transform,
                        rl::math::QuaternionToMatrix(
                                rl::math::QuaternionFromEuler(
                                        rotation[0],
                                        rotation[1],
                                        rotation[2]
                                )
                        )
                );
                transform = rl::math::MatrixMultiply(
                        transform,
                        rl::math::MatrixTranslate(
                                position[0],
                                position[1],
                                position[2]
                        )
                );

                // One half of X- billboard/sprite
                rl::DrawMesh(mesh, material, transform);

                transform = rl::math::MatrixMultiply(
                        transform,
                        rl::math::MatrixRotateY(PI/2)
                );

                // Second half of X- billboard/sprite
                rl::DrawMesh(mesh, material, transform);

                rl::UnloadMesh(mesh);
        }

        void EndDrawing() override {
                rl::EndMode3D();
                rl::EndDrawing();
        }

};

#endif // _OPENMAP_RAYLIB_DRAWER_HPP