#ifndef _OPENMAP_EDITOR_HPP
#define _OPENMAP_EDITOR_HPP

#include <src/INTERFACES/editor/IEditor.hpp>

#include <libs/raylib/include/raylib.h>


class Editor: public IEditor {

private:
        const float MOUSE_SENSITIVITY = 0.1f;
        const float MIN_CAMERA_MOVE_SPEED = 1.0f;

        const IInput::MouseButton BTN_CAMERA_TOGGLE = IInput::MouseButton::RIGHT;

        const IInput::Keycode KEY_CAMERA_FORWARD = IInput::Keycode::W;
        const IInput::Keycode KEY_CAMERA_BACK = IInput::Keycode::S;
        const IInput::Keycode KEY_CAMERA_RIGHT = IInput::Keycode::A;
        const IInput::Keycode KEY_CAMERA_LEFT = IInput::Keycode::D;
        const IInput::Keycode KEY_CAMERA_UP = IInput::Keycode::SPACE;
        const IInput::Keycode KEY_CAMERA_DOWN = IInput::Keycode::LEFT_CONTROL;


        float camera_move_speed = MIN_CAMERA_MOVE_SPEED;


public:
        int Run(
                IDraw&& drawer,
                IInput&& input
        ) override {
                // TEMP; TEST:
                //IDraw::Model* test_model = drawer.LoadModel("viking_room.obj");
                IDraw::Model* test_sprite = drawer.LoadSprite("viking_room.png");
                float test_model_position[3] = {0.0f, 0.0f, 10.0f};
                float test_model_rotation[3] = {0.0f, 0.0f, 0.0f};
                float test_model_scale[3] = {1.0f, 1.0f, 1.0f};

                while (!drawer.WindowShouldClose()) {

                        std::tuple<int, int> mouse_delta = input.GetMouseDelta();

                        std::tuple<int, int> mouse_wheel_delta = input.GetMouseWheelDelta();
                        camera_move_speed += std::get<1>(mouse_wheel_delta);
                        if (camera_move_speed < MIN_CAMERA_MOVE_SPEED) camera_move_speed = MIN_CAMERA_MOVE_SPEED;

                        if (input.IsMouseButtonDown(BTN_CAMERA_TOGGLE)) {
                                input.LockCursor();
                                drawer.UpdateCamera(
                                        new float[3]{
                                                (float)(input.IsKeyDown(KEY_CAMERA_FORWARD) -
                                                        input.IsKeyDown(KEY_CAMERA_BACK)) * camera_move_speed,
                                                (float)(input.IsKeyDown(KEY_CAMERA_LEFT) -
                                                        input.IsKeyDown(KEY_CAMERA_RIGHT)) * camera_move_speed,
                                                (float)(input.IsKeyDown(KEY_CAMERA_UP) -
                                                        input.IsKeyDown(KEY_CAMERA_DOWN)) * camera_move_speed
                                        },
                                        new float[3]{
                                                (float)std::get<0>(mouse_delta) * MOUSE_SENSITIVITY,
                                                (float)std::get<1>(mouse_delta) * MOUSE_SENSITIVITY,
                                                0.0f
                                        }
                                );
                        }
                        else {
                                input.UnlockCursor();
                        }

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