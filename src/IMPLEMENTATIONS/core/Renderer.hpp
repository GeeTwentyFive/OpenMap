#ifndef _OPENMAP_RENDERER_HPP
#define _OPENMAP_RENDERER_HPP

#include <src/INTERFACES/core/IRenderer.hpp>

#include <stdexcept>
#include <libs/raylib-cpp/include/raylib-cpp.hpp>

class Renderer : public IRenderer {

private:
        const double CAMERA_NEAR = 1.0f;
        const double CAMERA_FAR = 100000.0f;
        const float CAMERA_FOV = 90.0f;


        raylib::Camera3D camera;


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
                        raylib::Color{0xFFFFFFFF}
                );
        }


public:
        Renderer() {

                camera = raylib::Camera3D(
                        raylib::Vector3{0.0f, 0.0f, 0.0f},
                        raylib::Vector3{0.0f, 0.0f, -1.0f},
                        raylib::Vector3{0.0f, 1.0f, 0.0f},
                        CAMERA_FOV,
                        CAMERA_PERSPECTIVE
                );

                rlSetClipPlanes(CAMERA_NEAR, CAMERA_FAR);

                rlDisableBackfaceCulling();

        }

        inline Model* Load(const std::string& path) override {
                raylib::Model* model = new raylib::Model;
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
                camera.Update(
                        Vector3{
                                movement[0],
                                movement[1],
                                movement[2]
                        },
                        Vector3{
                                rotation[0],
                                rotation[1],
                                0.0f
                        }
                );
                camera.position = Vector3Add(
                        camera.position,
                        Vector3Normalize(Vector3Subtract(camera.target, camera.position))
                );
        } 

        inline void ClearFramebuffer() override {
                ::ClearBackground(raylib::Color{0xFFFFFFFF});
        }

        inline void Render(
                const std::vector<MapObjectInstance>& map_object_instances,
                const std::vector<size_t>& selected_map_objects_indices
        ) override {
                camera.BeginMode();
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
                camera.EndMode();
        }

};

#endif // _OPENMAP_RENDERER_HPP