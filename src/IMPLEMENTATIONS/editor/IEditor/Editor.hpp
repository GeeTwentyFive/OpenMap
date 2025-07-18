#ifndef _OPENMAP_EDITOR_HPP
#define _OPENMAP_EDITOR_HPP

#include <src/INTERFACES/editor/IEditor.hpp>

#include <stdexcept>
#include <array>
#include <fstream>
#include <libs/chaiscript/chaiscript.hpp>
#include <libs/nlohmann/json.hpp>


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
                std::vector<std::string> extra_data;
        };

        struct MapObjectInstance {
                std::string name;
                IDraw::Model* model;
                float pos[3];
                float rot[3];
                float scale[3];
                std::vector<std::string> extra_data;
        };


        std::map<std::string, MapObject> map_objects;
        std::vector<MapObjectInstance> map_object_instances;

        float camera_move_speed = MIN_CAMERA_MOVE_SPEED;

        IDraw* _drawer = nullptr;
        IInput* _input = nullptr;
        const char* _export_script_file_name = nullptr;


        void InstantiateMapObject(std::string name) {

                if (map_objects.count(name) == 0) throw std::runtime_error(
                        std::string("ERROR: InstantiateMapObject(): MapObject not found: ") +
                        '"' + name + '"'
                );

                MapObjectInstance map_object_instance{
                        .name = name,
                        .model = map_objects[name].model,
                        .extra_data = map_objects[name].extra_data
                };

                std::array<float, 3> camera_position = _drawer->GetCameraPosition();
                map_object_instance.pos[0] = camera_position[0];
                map_object_instance.pos[1] = camera_position[1];
                map_object_instance.pos[2] = camera_position[2];

                map_object_instance.scale[0] =
                        map_object_instance.scale[1] =
                                map_object_instance.scale[2] = 1.0f;

                map_object_instances.push_back(map_object_instance);

        }


public:
        int Run(
                IDraw* drawer,
                IInput* input,
                const char* config_file_name,
                const char* export_script_file_name = 0
        ) override {
                _drawer = drawer;
                _input = input;
                _export_script_file_name = export_script_file_name;

                // Init config.chai ChaiScript -> execute and get MapObjects
                chaiscript::ChaiScript config_chai;
                config_chai.add_global_const(chaiscript::const_var(MapObjectType::MODEL), "MODEL");
                config_chai.add_global_const(chaiscript::const_var(MapObjectType::SPRITE), "SPRITE");
                config_chai.add(chaiscript::type_conversion<
                                std::vector<chaiscript::Boxed_Value>,
                                std::vector<std::string>
                        >([](const std::vector<chaiscript::Boxed_Value>& bv){
                                std::vector<std::string> result;
                                for (chaiscript::Boxed_Value item : bv) {
                                        result.push_back(chaiscript::boxed_cast<std::string>(item));
                                }
                                return result;
                        })
                );
                config_chai.add(chaiscript::fun([this](
                                std::string name,
                                MapObjectType type,
                                std::string path,
                                std::vector<std::string> extra_data
                        ){
                                this->AddMapObject(name, type, path, extra_data);
                        }),
                        "AddMapObject"
                );
                config_chai.add(chaiscript::fun([this](
                                std::string name,
                                MapObjectType type,
                                std::string path
                        ){
                                this->AddMapObject(name, type, path);
                        }),
                        "AddMapObject"
                );
                try { config_chai.eval_file(config_file_name); }
                catch(const std::exception& e) {
                        throw std::runtime_error(
                                std::string("ERROR: Failed to read OpenMap config: ") +
                                '"' + config_file_name + '"' +
                                "\nChaiScript's error: " + e.what()
                        );
                }

                // Main loop
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
                                for (MapObjectInstance map_object : map_object_instances) {
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

                Export("MAP.CUSTOM_FORMAT"); // TEMP; TEST

                return 0;

        }

        inline void AddMapObject(
                std::string name,
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
                map_object.extra_data = extra_data;
                map_objects[name] = map_object;
        }


        inline void Export(std::string path) override {
                chaiscript::ChaiScript export_chai;
                try { export_chai.eval_file(_export_script_file_name); }
                catch(const std::exception& e) {
                        std::cout << "Export script " <<
                                '"' << _export_script_file_name << '"' <<
                                " not found or failed. Exporting as JSON." <<
                                "\n^ ChaiScript's error: " << e.what() <<
                                std::endl;

                        nlohmann::json j_array = nlohmann::json::array();
                        for (MapObjectInstance m : map_object_instances) {
                                nlohmann::json j_instance;
                                j_instance["name"] = m.name;
                                j_instance["position"] = {m.pos[0], m.pos[1], m.pos[2]};
                                j_instance["rotation"] = {m.rot[0], m.rot[1], m.rot[2]};
                                j_instance["scale"] = {m.scale[0], m.scale[1], m.scale[2]};
                                j_instance["extra_data"] = m.extra_data;

                                j_array.push_back(j_instance);
                        }

                        std::ofstream out_file(path);
                        out_file << j_array.dump(4);
                        out_file.close();

                        return;
                }

                // TODO: Define MapObjectInstance in export_chai script

                std::function<std::string(std::vector<MapObjectInstance>)> get_export_map_data;
                try { get_export_map_data = export_chai.eval<std::function<std::string(std::vector<MapObjectInstance>)>>("export"); }
                catch(const std::exception& e) {
                        throw std::runtime_error(
                                std::string("ERROR: Failed to get function \"export\" from export script: ") +
                                '"' + _export_script_file_name + '"' +
                                "\n^ ChaiScript's error: " + e.what()
                        );
                }

                std::string export_map_data = get_export_map_data(map_object_instances);

                std::ofstream out_file(path);
                out_file << export_map_data;
                out_file.close();

        }

};

#endif // _OPENMAP_EDITOR_HPP