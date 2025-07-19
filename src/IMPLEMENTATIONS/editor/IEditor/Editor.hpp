#ifndef _OPENMAP_EDITOR_HPP
#define _OPENMAP_EDITOR_HPP

#include <src/INTERFACES/editor/IEditor.hpp>

#include <unordered_map>
#include <stdexcept>
#include <array>
#include <fstream>
#include <libs/chaiscript/chaiscript.hpp>
#include <libs/nlohmann/json.hpp>


class Editor: public IEditor {

private:
        const float MOUSE_SENSITIVITY = 0.1f;
        const float MIN_CAMERA_MOVE_SPEED = 0.1f;
        const char* QUIT_SAVE_FILE_NAME = "QUITSAVE";

        const IInput::MouseButton BTN_CAMERA_TOGGLE = IInput::MouseButton::RIGHT;
        const IInput::Keycode KEY_CAMERA_FORWARD = IInput::Keycode::W;
        const IInput::Keycode KEY_CAMERA_BACK = IInput::Keycode::S;
        const IInput::Keycode KEY_CAMERA_RIGHT = IInput::Keycode::A;
        const IInput::Keycode KEY_CAMERA_LEFT = IInput::Keycode::D;
        const IInput::Keycode KEY_CAMERA_UP = IInput::Keycode::SPACE;
        const IInput::Keycode KEY_CAMERA_DOWN = IInput::Keycode::LEFT_CONTROL;


        struct MapObject {
                IDraw::Model* model;
                std::string extra_data;
        };

        struct MapObjectInstance {
                std::string name;
                IDraw::Model* model;
                std::array<float, 3> pos;
                std::array<float, 3> rot;
                std::array<float, 3> scale;
                std::string extra_data;
        };


        std::unordered_map<std::string, MapObject> map_objects;
        std::vector<MapObjectInstance> map_object_instances;

        float camera_move_speed = MIN_CAMERA_MOVE_SPEED;

        IDraw* _drawer = nullptr;
        IInput* _input = nullptr;

        chaiscript::ChaiScript config_chai;


        void InitConfigScript() {
                // AddMapObject()
                config_chai.add_global_const(chaiscript::const_var(MapObjectType::MODEL), "MODEL");
                config_chai.add_global_const(chaiscript::const_var(MapObjectType::SPRITE), "SPRITE");
                // (to convert from chaiscript::Boxed_Value to std::string
                // when calling C++ from ChaiScript with string as argument)
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
                                std::string extra_data
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

                // MapObjectInstance
                config_chai.add(chaiscript::user_type<MapObjectInstance>(), "MapObjectInstance");
                config_chai.add(chaiscript::fun(&MapObjectInstance::name), "name");
                config_chai.add(chaiscript::user_type<std::array<float, 3>>(), "Vector3");
                config_chai.add(
                        chaiscript::fun(
                                [](std::array<float, 3>& array, size_t index) -> float& {
                                        return array[index];
                                }
                        ),
                        "[]"
                );
                config_chai.add(
                        chaiscript::fun(
                                [](float f, std::string s) -> std::string {
                                        return std::to_string(f) + s;
                                }
                        ),
                        "+"
                );
                config_chai.add(
                        chaiscript::fun(
                                [](std::string s, float f) -> std::string {
                                        return s + std::to_string(f);
                                }
                        ),
                        "+"
                );
                config_chai.add(chaiscript::fun(&MapObjectInstance::pos), "pos");
                config_chai.add(chaiscript::fun(&MapObjectInstance::rot), "rot");
                config_chai.add(chaiscript::fun(&MapObjectInstance::scale), "scale");
                config_chai.add(chaiscript::fun(&MapObjectInstance::extra_data), "extra_data");
                chaiscript::ModulePtr m = chaiscript::ModulePtr(new chaiscript::Module());
                chaiscript::bootstrap::standard_library::vector_type<std::vector<MapObjectInstance>>("VectorMapObjectInstance", *m);
                config_chai.add(m);
        }


public:
        int Run(
                IDraw* drawer,
                IInput* input,
                const char* config_script_file_name
        ) override {
                _drawer = drawer;
                _input = input;

                InitConfigScript();

                // Execute config script -> get MapObjects
                try { config_chai.eval_file(config_script_file_name); }
                catch(const std::exception& e) {
                        throw std::runtime_error(
                                std::string("ERROR: Failed to execute config script: ") +
                                '"' + config_script_file_name + '"' +
                                "\nChaiScript's error: " + e.what()
                        );
                }

                // TEMP; TEST:
                std::array<float, 3> camera_pos = _drawer->GetCameraPosition();
                InstantiateMapObject(
                        "Viking Room Model",
                        camera_pos,
                        {0.0f, 0.0f, 0.0f},
                        {1.0f, 1.0f, 1.0f}
                );
                InstantiateMapObject(
                        "Viking Room Sprite",
                        camera_pos,
                        {0.0f, 0.0f, 0.0f},
                        {1.0f, 1.0f, 1.0f},
                        "TEST_FIELD_1: TEST_DATA_1\nTEST_FIELD_2: TEST_DATA_2"
                );

                // Main loop
                while (!_drawer->WindowShouldClose()) {

                        std::tuple<int, int> mouse_delta = _input->GetMouseDelta();

                        std::tuple<int, int> mouse_wheel_delta = _input->GetMouseWheelDelta();
                        camera_move_speed += std::get<1>(mouse_wheel_delta);
                        if (camera_move_speed < MIN_CAMERA_MOVE_SPEED) camera_move_speed = MIN_CAMERA_MOVE_SPEED;

                        if (_input->IsMouseButtonDown(BTN_CAMERA_TOGGLE)) {
                                _input->LockCursor();
                                _drawer->UpdateCamera(
                                        {
                                                (float)(_input->IsKeyDown(KEY_CAMERA_FORWARD) -
                                                        _input->IsKeyDown(KEY_CAMERA_BACK)) * camera_move_speed,
                                                (float)(_input->IsKeyDown(KEY_CAMERA_LEFT) -
                                                        _input->IsKeyDown(KEY_CAMERA_RIGHT)) * camera_move_speed,
                                                (float)(_input->IsKeyDown(KEY_CAMERA_UP) -
                                                        _input->IsKeyDown(KEY_CAMERA_DOWN)) * camera_move_speed
                                        },
                                        {
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

                Save(QUIT_SAVE_FILE_NAME);

                return 0;

        }

        inline void AddMapObject(
                std::string name,
                MapObjectType type,
                std::string path,
                std::string extra_data = {}
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

        inline void InstantiateMapObject(
                std::string name,
                std::array<float, 3> pos,
                std::array<float, 3> rot,
                std::array<float, 3> scale,
                std::string extra_data = {}
        ) override {

                if (map_objects.count(name) == 0) throw std::runtime_error(
                        std::string("ERROR: InstantiateMapObject(): MapObject not found: ") +
                        '"' + name + '"'
                );

                MapObjectInstance map_object_instance{
                        .name = name,
                        .model = map_objects[name].model,
                };

                if (extra_data.empty()) {
                        map_object_instance.extra_data = map_objects[name].extra_data;
                }
                else map_object_instance.extra_data = extra_data;

                map_object_instance.pos = pos;
                map_object_instance.rot = rot;
                map_object_instance.scale = scale;

                map_object_instances.push_back(map_object_instance);

        }

        inline void Save(std::string path) override {
                nlohmann::json j_array = nlohmann::json::array();
                for (MapObjectInstance m : map_object_instances) {
                        nlohmann::json j_instance;
                        j_instance["name"] = m.name;
                        j_instance["position"] = m.pos;
                        j_instance["rotation"] = m.rot;
                        j_instance["scale"] = m.scale;
                        j_instance["extra_data"] = m.extra_data;

                        j_array.push_back(j_instance);
                }

                std::ofstream out_file(path);
                out_file << j_array.dump(4);
                out_file.close();
        }
        inline void Load(std::string path) override {
                // TODO: Don't crash app upon failed load, just print error & return
        }
        inline void Clear() override {
                map_object_instances.clear();
        }

        inline void Export(std::string path) override {
                std::function<std::string(std::vector<MapObjectInstance>)> get_export_data;
                try { get_export_data = config_chai.eval<std::function<std::string(std::vector<MapObjectInstance>)>>("export"); }
                catch(const std::exception& e) {
                        std::cout << "Export function \"export\" not found or failed. Exporting as JSON." <<
                                "\n^ ChaiScript's error: " << e.what() <<
                                std::endl;

                        nlohmann::json j_array = nlohmann::json::array();
                        for (MapObjectInstance m : map_object_instances) {
                                nlohmann::json j_instance;
                                j_instance["name"] = m.name;
                                j_instance["position"] = m.pos;
                                j_instance["rotation"] = m.rot;
                                j_instance["scale"] = m.scale;
                                j_instance["extra_data"] = m.extra_data;

                                j_array.push_back(j_instance);
                        }

                        std::ofstream out_file(path);
                        out_file << j_array.dump(4);
                        out_file.close();

                        return;
                }

                std::string export_map_data = get_export_data(map_object_instances);

                std::ofstream out_file(path);
                out_file << export_map_data;
                out_file.close();

        }

};

#endif // _OPENMAP_EDITOR_HPP