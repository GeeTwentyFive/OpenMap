#ifndef _OPENMAP_EDITOR_HPP
#define _OPENMAP_EDITOR_HPP

#include <src/INTERFACES/editor/IEditor.hpp>


class Editor: public IEditor {

private:
        const float MOUSE_SENSITIVITY = 0.1f;
        const float MIN_CAMERA_MOVE_SPEED = 0.1f;

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
                                        new float[2]{
                                                (float)std::get<0>(mouse_delta) * MOUSE_SENSITIVITY,
                                                (float)std::get<1>(mouse_delta) * MOUSE_SENSITIVITY
                                        }
                                );
                        }
                        else {
                                input.UnlockCursor();
                        }

                        drawer.BeginDrawing();
                        {

                                // TODO: Draw added MapObject's

                        }
                        drawer.EndDrawing();

                }

                // TODO: Prompt to save before exiting if unsaved work

                return 0;

        }

};

#endif // _OPENMAP_EDITOR_HPP