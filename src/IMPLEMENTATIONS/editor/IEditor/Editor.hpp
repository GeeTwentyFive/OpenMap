#ifndef _OPENMAP_EDITOR_HPP
#define _OPENMAP_EDITOR_HPP

#include <src/INTERFACES/editor/IEditor.hpp>

#include <stdexcept>
#include <libs/chaiscript/chaiscript.hpp>


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


        struct MapObject {
                IDraw::Model* model;
                float pos[3];
                float rot[3];
                float scale[3];
                std::vector<std::string> extra_data;
        };


        std::vector<MapObject> map_objects;

        float camera_move_speed = MIN_CAMERA_MOVE_SPEED;


        IDraw* _drawer = nullptr;
        IInput* _input = nullptr;
        chaiscript::ChaiScript* chai;


public:
        int Run(
                IDraw* drawer,
                IInput* input,
                const char* config_file_name,
                const char* export_file_name = 0
        ) override {
                _drawer = drawer;
                _input = input;
                chai = new chaiscript::ChaiScript;

                // Init MapObjects via ChaiScript config
                try { chai->eval_file(config_file_name); }
                catch(const std::exception& e) {
                        throw std::runtime_error(
                                std::string("ERROR: Failed to read OpenMap config: ") +
                                '"' + config_file_name + '"' +
                                "\nChaiScript's error: " + e.what()
                        );
                }

                while (!_drawer->WindowShouldClose()) {

                        std::tuple<int, int> mouse_delta = _input->GetMouseDelta();

                        std::tuple<int, int> mouse_wheel_delta = _input->GetMouseWheelDelta();
                        camera_move_speed += std::get<1>(mouse_wheel_delta);
                        if (camera_move_speed < MIN_CAMERA_MOVE_SPEED) camera_move_speed = MIN_CAMERA_MOVE_SPEED;

                        if (_input->IsMouseButtonDown(BTN_CAMERA_TOGGLE)) {
                                _input->LockCursor();
                                _drawer->UpdateCamera(
                                        new float[3]{
                                                (float)(_input->IsKeyDown(KEY_CAMERA_FORWARD) -
                                                        _input->IsKeyDown(KEY_CAMERA_BACK)) * camera_move_speed,
                                                (float)(_input->IsKeyDown(KEY_CAMERA_LEFT) -
                                                        _input->IsKeyDown(KEY_CAMERA_RIGHT)) * camera_move_speed,
                                                (float)(_input->IsKeyDown(KEY_CAMERA_UP) -
                                                        _input->IsKeyDown(KEY_CAMERA_DOWN)) * camera_move_speed
                                        },
                                        new float[2]{
                                                (float)std::get<0>(mouse_delta) * MOUSE_SENSITIVITY,
                                                (float)std::get<1>(mouse_delta) * MOUSE_SENSITIVITY
                                        }
                                );
                        }
                        else {
                                _input->UnlockCursor();
                        }


                        _drawer->BeginDrawing();
                        {
                                for (MapObject map_object : map_objects) {
                                        _drawer->Draw(
                                                map_object.model,
                                                map_object.pos,
                                                map_object.rot,
                                                map_object.scale
                                        );
                                }
                        }
                        _drawer->EndDrawing();

                }

                // TODO: Prompt to save before exiting if unsaved work

                return 0;

        }

        inline void AddMapObject(
                MapObjectType type,
                std::string path,
                std::vector<std::string> extra_data = {}
        ) override {
                MapObject map_object{};
                switch (type) {
                        case MapObjectType::MODEL:
                        {
                                map_object.model = _drawer->LoadModel(path.c_str());
                        }
                        break;

                        case MapObjectType::SPRITE:
                        {
                                map_object.model = _drawer->LoadSprite(path.c_str());
                        }
                        break;
                }
                map_object.scale[0] = map_object.scale[1] = map_object.scale[2] = 1.0f;
                map_object.extra_data = extra_data;
                map_objects.push_back(map_object);
        }

        inline void Export(std::string path) override {
                // TODO:
                // - Check if "export.chai" exists
                //      - ^ if so:
                //              - call export(map_objects)
                //              - ^ extract string output as .c_str()
                //              - ^ write to <path> file
                //      - ^ if not: export as JSON
        }

};

#endif // _OPENMAP_EDITOR_HPP