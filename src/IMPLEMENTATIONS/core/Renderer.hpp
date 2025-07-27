#ifndef _OPENMAP_RENDERER_HPP
#define _OPENMAP_RENDERER_HPP

#include <src/INTERFACES/core/IRenderer.hpp>

#include <stdexcept>
#include <libs/raylib/include/raylib.h>
#define RAYMATH_STATIC_INLINE
#include <libs/raylib/include/raymath.h>
#include <libs/raylib/include/rlgl.h>

class Renderer : public IRenderer {

private:
        const double CAMERA_NEAR = 1.0f;
        const double CAMERA_FAR = 100000.0f;
        const float CAMERA_FOV = 90.0f;


        Camera camera;


        void DrawModel(
                Model* model,
                std::array<float, 3> position,
                std::array<float, 3> rotation,
                std::array<float, 3> scale
        ) {
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
                        Color{0xFF, 0xFF, 0xFF, 0xFF}
                );
        }


public:
        Renderer() {

                camera.position = Vector3{0.0f, 0.0f, 0.0f};
                camera.target = Vector3{0.0f, 0.0f, 1.0f};
                camera.up = Vector3{0.0f, 1.0f, 0.0f};
                camera.fovy = CAMERA_FOV;
                camera.projection = CAMERA_PERSPECTIVE;

                rlSetClipPlanes(CAMERA_NEAR, CAMERA_FAR);

        }

        inline Model* Load(const std::string& path) override {
                ::Model* model = new ::Model;
                *model = ::LoadModel(path.c_str());
                if (model->meshCount == 0) throw std::runtime_error(
                        std::string("ERROR: Failed to load model: ") + path
                );
                return (Model*)model;
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
                        1.0f
                );
                camera.position = Vector3Add(
                        camera.position,
                        Vector3Normalize(Vector3Subtract(camera.target, camera.position))
                );
        } 

        inline void ClearFramebuffer() override {
                ::ClearBackground(Color{0x00, 0x00, 0x00, 0xFF});
        }

        inline void Render(
                const std::vector<MapObjectInstance>& map_object_instances,
                const std::vector<size_t>& selected_map_objects_indices
        ) override {
                rlDisableBackfaceCulling();
                ::BeginMode3D(camera);
                {
                        for (MapObjectInstance map_object : map_object_instances) {
                                DrawModel(
                                        map_object.model,
                                        map_object.pos,
                                        map_object.rot,
                                        map_object.scale
                                );
                        }

                        // TODO: Highlight selected
                }
                ::EndMode3D();
        }

};

#endif // _OPENMAP_RENDERER_HPP