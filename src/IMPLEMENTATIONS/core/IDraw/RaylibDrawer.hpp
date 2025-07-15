#ifndef _OPENMAP_RAYLIB_DRAWER_HPP
#define _OPENMAP_RAYLIB_DRAWER_HPP

#include <src/INTERFACES/core/IDraw.hpp>

#include <libs/raylib/include/raylib.h>
#define RAYMATH_STATIC_INLINE
#include <libs/raylib/include/raymath.h>
#include <libs/raylib/include/rlgl.h>


class RaylibDrawer: public IDraw {

private:
        Camera camera;

public:
        inline RaylibDrawer(
                ConfigFlags window_config_flags = FLAG_MSAA_4X_HINT,
                int window_min_width = 1280,
                int window_min_height = 720,
                float camera_fov = 90
        ) {

                SetConfigFlags(window_config_flags);

                InitWindow(
                        window_min_width,
                        window_min_height,
                        ""
                );

                SetTargetFPS(
                        GetMonitorRefreshRate(GetCurrentMonitor()) * 2
                );

                SetWindowMinSize(window_min_width, window_min_height);

                camera.position = Vector3{0.0f, 0.0f, 0.0f};
                camera.target = Vector3{0.0f, 0.0f, 1.0f};
                camera.up = Vector3{0.0f, 1.0f, 0.0f};
                camera.fovy = camera_fov;
                camera.projection = CAMERA_PERSPECTIVE;

        }

        inline ~RaylibDrawer() {
                CloseWindow();
        }


        inline Model* LoadModel(const char* target_file_path) override {
                ::Model* model = new ::Model;
                *model = ::LoadModel(target_file_path);
                return (Model*)model;
        }
        inline void UnloadModel(Model* model) override {
                ::UnloadModel(*(::Model*)model);
                delete model;
        }

        inline Sprite* LoadSprite(const char* target_file_path) override {
                Texture2D* sprite = new Texture2D;
                *sprite = LoadTexture(target_file_path);
                return (Sprite*)sprite;
        }
        inline void UnloadSprite(Sprite* sprite) override {
                UnloadTexture(*(Texture2D*)sprite);
                delete sprite;
        }


        inline bool WindowShouldClose() override {
                return ::WindowShouldClose();
        }

        inline void BeginDrawing(int clear_color = 0xffffffff) override {
                UpdateCamera(&camera, CAMERA_FREE);
                ::BeginDrawing();
                ClearBackground(
                        Color{
                                (unsigned char)((clear_color & 0xff000000) >> 24),
                                (unsigned char)((clear_color & 0x00ff0000) >> 16),
                                (unsigned char)((clear_color & 0x0000ff00) >> 8),
                                (unsigned char)((clear_color & 0x000000ff))
                        }
                );
        }

        inline void Draw(
                Model* model,
                float position[3],
                float rotation[3],
                float scale[3],
                int color_tint = 0xffffffff
        ) override {
                BeginMode3D(camera);

                ::Model _model = *(::Model*)model;

                // Have to manually rotate since DrawModelEx() doesn't support multi-axis rotation...
                _model.transform = MatrixMultiply(
                        _model.transform,

                        QuaternionToMatrix(
                                QuaternionFromEuler(
                                        rotation[0],
                                        rotation[1],
                                        rotation[2]
                                )
                        )
                );

                DrawModelEx(
                        _model,
                        Vector3{
                                position[0],
                                position[1],
                                position[2]
                        },
                        Vector3{0}, 0,
                        Vector3{
                                scale[0],
                                scale[1],
                                scale[2]
                        },
                        Color{
                                (unsigned char)((color_tint & 0xff000000) >> 24),
                                (unsigned char)((color_tint & 0x00ff0000) >> 16),
                                (unsigned char)((color_tint & 0x0000ff00) >> 8),
                                (unsigned char)((color_tint & 0x000000ff))
                        }
                );

                EndMode3D();
        }

        inline void Draw(
                Sprite* sprite,
                float position[3],
                float rotation[3],
                float scale[3],
                int color_tint = 0xffffffff
        ) override {
                BeginMode3D(camera);

                Texture2D _sprite = *(Texture2D*)sprite;

                Mesh mesh = GenMeshPlane(
                        _sprite.width,
                        _sprite.height,
                        1, 1
                );

                Material material = LoadMaterialDefault();
                material.maps[MATERIAL_MAP_DIFFUSE].texture = _sprite;

                Matrix transform = MatrixIdentity();
                transform = MatrixMultiply(
                        transform,
                        MatrixScale(
                                scale[0],
                                scale[1],
                                scale[2]
                        )
                );
                transform = MatrixMultiply(
                        transform,
                        QuaternionToMatrix(
                                QuaternionFromEuler(
                                        rotation[0],
                                        rotation[1],
                                        rotation[2]
                                )
                        )
                );
                transform = MatrixMultiply(
                        transform,
                        MatrixTranslate(
                                position[0],
                                position[1],
                                position[2]
                        )
                );

                rlDisableBackfaceCulling();
                DrawMesh(mesh, material, transform);
                rlEnableBackfaceCulling();

                UnloadMesh(mesh);

                EndMode3D();
        }

        inline void EndDrawing() override {
                ::EndDrawing();
        }

};

#endif // _OPENMAP_RAYLIB_DRAWER_HPP