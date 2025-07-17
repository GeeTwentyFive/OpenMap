#ifndef _OPENMAP_EDITOR_HPP
#define _OPENMAP_EDITOR_HPP

#include <src/INTERFACES/editor/IEditor.hpp>

#include <iostream> // TEMP; TEST
#include <libs/raylib/include/raylib.h>


class Editor: public IEditor {

private:
        const float MOUSE_SENSITIVITY = 0.1f;

        const IInput::Keycode KEY_CAMERA_FORWARD = IInput::Keycode::W;
        const IInput::Keycode KEY_CAMERA_BACK = IInput::Keycode::S;
        const IInput::Keycode KEY_CAMERA_RIGHT = IInput::Keycode::A;
        const IInput::Keycode KEY_CAMERA_LEFT = IInput::Keycode::D;
        const IInput::Keycode KEY_CAMERA_UP = IInput::Keycode::SPACE;
        const IInput::Keycode KEY_CAMERA_DOWN = IInput::Keycode::LEFT_CONTROL;
        const IInput::Keycode KEY_CAMERA_TOGGLE = IInput::Keycode::MB_RIGHT;


        int camera_movement_direction[3] = {0};
        float camera_move_speed = 1.0f;


        void InitInputs(IInput &input) {
                input.Map(
                        KEY_CAMERA_FORWARD,
                        [
                                &camera_movement_direction = this->camera_movement_direction
                        ](){
                                camera_movement_direction[2] += 1; std::cout << "FORWARD" << std::endl;
                        }
                );

                input.Map(
                        KEY_CAMERA_BACK,
                        [
                                &camera_movement_direction = this->camera_movement_direction
                        ](){
                                camera_movement_direction[2] -= 1;
                        }
                );

                input.Map(
                        KEY_CAMERA_RIGHT,
                        [
                                &camera_movement_direction = this->camera_movement_direction
                        ](){
                                camera_movement_direction[0] += 1;
                        }
                );

                input.Map(
                        KEY_CAMERA_LEFT,
                        [
                                &camera_movement_direction = this->camera_movement_direction
                        ](){
                                camera_movement_direction[0] -= 1;
                        }
                );

                input.Map(
                        KEY_CAMERA_UP,
                        [
                                &camera_movement_direction = this->camera_movement_direction
                        ](){
                                camera_movement_direction[1] += 1;
                        }
                );

                input.Map(
                        KEY_CAMERA_DOWN,
                        [
                                &camera_movement_direction = this->camera_movement_direction
                        ](){
                                camera_movement_direction[1] -= 1;
                        }
                );
        }


public:
        int Run(
                IDraw&& drawer,
                IInput&& input
        ) override {

                InitInputs(input);


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

                        drawer.UpdateCamera(
                                new float[3]{
                                        (float)camera_movement_direction[0] * camera_move_speed,
                                        (float)camera_movement_direction[1] * camera_move_speed,
                                        (float)camera_movement_direction[2] * camera_move_speed
                                },
                                new float[3]{
                                        (float)std::get<0>(mouse_delta) * MOUSE_SENSITIVITY,
                                        (float)std::get<1>(mouse_delta) * MOUSE_SENSITIVITY,
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