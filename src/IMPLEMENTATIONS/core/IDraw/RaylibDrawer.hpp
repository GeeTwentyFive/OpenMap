#ifndef _OPENMAP_RAYLIB_DRAWER_HPP
#define _OPENMAP_RAYLIB_DRAWER_HPP

#include <src/INTERFACES/core/IDraw.hpp>

#include <stdexcept>
#include <array>
#include <float.h>
#include <libs/raylib/include/raylib.h>
#define RAYMATH_STATIC_INLINE
#include <libs/raylib/include/raymath.h>
#include <libs/raylib/include/rlgl.h>
#include <libs/raylib-gizmo/raygizmo.h>


class RaylibDrawer: public IDraw {

private:
        const double CAMERA_NEAR = 1.0f;
	const double CAMERA_FAR = 100000.0f;


        Camera camera;

        struct {
                Transform transform;
                std::array<float, 3>* target_pos;
                std::array<float, 3>* target_rot;
                std::array<float, 3>* target_scale;
        } gizmo_state;


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

                rlSetClipPlanes(CAMERA_NEAR, CAMERA_FAR);

                rlDisableBackfaceCulling();

                SetExitKey(0);

        }

        inline ~RaylibDrawer() {
                CloseWindow();
        }

        inline Model* LoadModel(const char* target_file_path) override {
                ::Model* model = new ::Model;
                *model = ::LoadModel(target_file_path);
                if (model->meshCount == 0) throw std::runtime_error(
                        std::string("ERROR: Failed to load model: ") + target_file_path
                );
                return (Model*)model;
        }

        inline Model* LoadSprite(const char* target_file_path) override {

                Texture2D sprite = LoadTexture(target_file_path);
                if (sprite.width == 0 && sprite.height == 0) throw std::runtime_error(
                        std::string("ERROR: Failed to load sprite: ") + target_file_path
                );

                Mesh mesh = GenMeshPlane(
                        sprite.width,
                        sprite.height,
                        1, 1
                );

                ::Model* model = new ::Model;
                *model = LoadModelFromMesh(mesh);
                model->materials->maps[MATERIAL_MAP_DIFFUSE].texture = sprite;

                return (Model*)model;

        }

        inline void Unload(Model* model) override {
                ::UnloadModel(*(::Model*)model);
                delete model;
        }

        inline bool WindowShouldClose() override {
                return ::WindowShouldClose();
        }

        inline std::pair<int, int> GetScreenResolution() override {
                return std::pair<int, int>{
                        GetScreenWidth(),
                        GetScreenHeight()
                };
        }

        inline void UpdateCamera(
                std::array<float, 3> movement,
                std::array<float, 2> rotation
        ) override {
                UpdateCameraPro(
                        &camera,
                        Vector3{
                                movement[0],
                                movement[1],
                                movement[2]
                        },
                        Vector3{
                                rotation[0],
                                rotation[1],
                                0.0f
                        },
                        1
                );
                camera.position = Vector3Add(
                        camera.position,
                        Vector3Normalize(Vector3Subtract(camera.target, camera.position))
                );
        }

        inline std::array<float, 3> GetCameraPosition() override {
                return std::array<float, 3>{
                        camera.position.x,
                        camera.position.y,
                        camera.position.z
                };
        }

        inline void BeginDrawing(int32_t clear_color = 0x000000ff) override {
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
        inline void BeginMode3D() override {
                ::BeginMode3D(camera);
        }

        inline void Draw(
                Model* model,
                std::array<float, 3> position,
                std::array<float, 3> rotation,
                std::array<float, 3> scale,
                int32_t color_tint = 0xffffffff
        ) override {
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
        }

        inline BoundingBox GetModelBoundingBox(Model* model) override {
                ::BoundingBox box = ::GetModelBoundingBox(*(::Model*)model);
                return BoundingBox{
                        {
                                box.min.x,
                                box.min.y,
                                box.min.z
                        },
                        {
                                box.max.x,
                                box.max.y,
                                box.max.z
                        }
                };
        }

        inline BoundingBox GetOffsetModelBoundingBox(
                Model* model,
                std::array<float, 3> translation_offset,
                std::array<float, 3> rotation_offset,
                std::array<float, 3> scale_offset
        ) override {
                BoundingBox box = GetModelBoundingBox(model);

                Matrix box_transform = MatrixMultiply(
                        MatrixMultiply(
                                MatrixScale(
                                        scale_offset[0],
                                        scale_offset[1],
                                        scale_offset[2]
                                ),
                                QuaternionToMatrix(
                                        QuaternionFromEuler(
                                                rotation_offset[0],
                                                rotation_offset[1],
                                                rotation_offset[2]
                                        )
                                )
                        ),
                        MatrixTranslate(
                                translation_offset[0],
                                translation_offset[1],
                                translation_offset[2]
                        )
                );

                // Get the 8 corners of the bounding box
                Vector3 corners[8] = {
                        { box.min[0], box.min[1], box.min[2] },
                        { box.min[0], box.min[1], box.max[2] },
                        { box.min[0], box.max[1], box.min[2] },
                        { box.min[0], box.max[1], box.max[2] },
                        { box.max[0], box.min[1], box.min[2] },
                        { box.max[0], box.min[1], box.max[2] },
                        { box.max[0], box.max[1], box.min[2] },
                        { box.max[0], box.max[1], box.max[2] },
                };

                // Transform all corners
                std::array<float, 3> box_min = { FLT_MAX, FLT_MAX, FLT_MAX };
                std::array<float, 3> box_max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
                for (int i = 0; i < 8; i++) {
                        Vector3 t = Vector3Transform(corners[i], box_transform);
                        if (t.x < box_min[0]) box_min[0] = t.x;
                        if (t.y < box_min[1]) box_min[1] = t.y;
                        if (t.z < box_min[2]) box_min[2] = t.z;
                        if (t.x > box_max[0]) box_max[0] = t.x;
                        if (t.y > box_max[1]) box_max[1] = t.y;
                        if (t.z > box_max[2]) box_max[2] = t.z;
                }

                return BoundingBox{ box_min, box_max };
        }

        inline void DrawBoundingBox(BoundingBox bounding_box, int32_t color = 0xffffffff) {
                ::DrawBoundingBox(
                        ::BoundingBox{
                                Vector3{
                                        bounding_box.min[0],
                                        bounding_box.min[1],
                                        bounding_box.min[2]
                                },
                                Vector3{
                                        bounding_box.max[0],
                                        bounding_box.max[1],
                                        bounding_box.max[2]
                                }
                        },
                        Color{
                                (unsigned char)((color & 0xff000000) >> 24),
                                (unsigned char)((color & 0x00ff0000) >> 16),
                                (unsigned char)((color & 0x0000ff00) >> 8),
                                (unsigned char)((color & 0x000000ff))
                        }
                );
        }

        inline Ray GetScreenToWorldRay(std::tuple<int, int> pos) override {
                ::Ray ray = ::GetScreenToWorldRay(
                        Vector2{(float)std::get<0>(pos), (float)std::get<1>(pos)},
                        camera
                );
                return Ray{
                        .pos = {
                                ray.position.x,
                                ray.position.y,
                                ray.position.z
                        },
                        .direction = {
                                ray.direction.x,
                                ray.direction.y,
                                ray.direction.z
                        }
                };
        }

        inline RayCollision GetRayCollisionBox(Ray ray, BoundingBox box) override {
                ::RayCollision collision = ::GetRayCollisionBox(
                        ::Ray{
                                .position = Vector3{
                                        ray.pos[0],
                                        ray.pos[1],
                                        ray.pos[2]
                                },
                                .direction = Vector3{
                                        ray.direction[0],
                                        ray.direction[1],
                                        ray.direction[2]
                                }
                        },
                        ::BoundingBox{
                                .min = Vector3{
                                        box.min[0],
                                        box.min[1],
                                        box.min[2]
                                },
                                .max = Vector3{
                                        box.max[0],
                                        box.max[1],
                                        box.max[2]
                                }
                        }
                );
                return RayCollision{
                        .hit = collision.hit,
                        .distance_to_hit = collision.distance,
                        .hit_point = {
                                collision.point.x,
                                collision.point.y,
                                collision.point.z
                        },
                        .hit_normal = {
                                collision.normal.x,
                                collision.normal.y,
                                collision.normal.z
                        }
                };
        }

        inline void HookGizmoTo(
                std::array<float, 3>* OUT_pos,
                std::array<float, 3>* OUT_rot,
                std::array<float, 3>* OUT_scale
        ) override {
                gizmo_state.target_pos = OUT_pos;
                gizmo_state.target_rot = OUT_rot;
                gizmo_state.target_scale = OUT_scale;

                gizmo_state.transform = Transform{
                        .translation = Vector3{
                                .x = (*gizmo_state.target_pos)[0],
                                .y = (*gizmo_state.target_pos)[1],
                                .z = (*gizmo_state.target_pos)[2]
                        },
                        .rotation = QuaternionFromEuler(
                                (*gizmo_state.target_rot)[0],
                                (*gizmo_state.target_rot)[1],
                                (*gizmo_state.target_rot)[2]
                        ),
                        .scale = Vector3{
                                .x = (*gizmo_state.target_scale)[0],
                                .y = (*gizmo_state.target_scale)[1],
                                .z = (*gizmo_state.target_scale)[2]
                        }
                };
        }

        inline void DrawAndUpdateGizmo() override {
                DrawGizmo3D(GIZMO_ALL, &gizmo_state.transform); // Updates gizmo as well
                rlDisableBackfaceCulling(); // Undoes internal rlEnableBackfaceCulling()

                (*gizmo_state.target_pos) = {
                        gizmo_state.transform.translation.x,
                        gizmo_state.transform.translation.y,
                        gizmo_state.transform.translation.z
                };
                Vector3 new_rot = QuaternionToEuler(gizmo_state.transform.rotation);
                (*gizmo_state.target_rot) = {
                        new_rot.x,
                        new_rot.y,
                        new_rot.z
                };
                (*gizmo_state.target_scale) = {
                        gizmo_state.transform.scale.x,
                        gizmo_state.transform.scale.y,
                        gizmo_state.transform.scale.z
                };
        }

        inline void EndMode3D() override {
                ::EndMode3D();
        }
        inline void EndDrawing() override {
                ::EndDrawing();
        }

};

#endif // _OPENMAP_RAYLIB_DRAWER_HPP