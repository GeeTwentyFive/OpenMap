#ifndef _OPENMAP_EDITOR_HPP
#define _OPENMAP_EDITOR_HPP

#include <src/INTERFACES/editor/IEditor.hpp>

#include <libs/raylib/include/raylib.h>


class Editor: public IEditor {

public:
        int Run(
                IDraw&& drawer
        ) override {

                //IDraw::Model* test_model = drawer.LoadModel("viking_room.obj");
                IDraw::Model* test_sprite = drawer.LoadSprite("viking_room.png");
                float test_model_position[3] = {0.0f, 0.0f, 10.0f};
                float test_model_rotation[3] = {0.0f, 0.0f, 0.0f};
                float test_model_scale[3] = {1.0f, 1.0f, 1.0f};

                while (!drawer.WindowShouldClose()) {

                        drawer.UpdateCamera(
                                new float[3]{
                                        0.0f, // TODO
                                        0.0f,
                                        0.0f
                                },
                                new float[3]{
                                        0.0f, // TODO: IInput + <-GetMouseDelta
                                        0.0f,
                                        0.0f
                                }
                        );

                        drawer.BeginDrawing();
                        {

                                drawer.Draw(
                                        test_sprite,
                                        test_model_position,
                                        test_model_rotation,
                                        test_model_scale,
                                        0x808080ff
                                );

                        }
                        drawer.EndDrawing();

                }

                return 0;

        }

};

#endif // _OPENMAP_EDITOR_HPP