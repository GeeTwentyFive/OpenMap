#ifndef _OPENMAP_EDITOR_HPP
#define _OPENMAP_EDITOR_HPP

#include <src/INTERFACES/editor/IEditor.hpp>

#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <deque>
#include <ranges>
#include <src/INTERFACES/core/IWindower.hpp>
#include <src/INTERFACES/core/IRenderer.hpp>
#include <src/INTERFACES/core/IGUI.hpp>
#include <src/INTERFACES/core/ISerializer.hpp>


class Editor : public IEditor {

private:
        const float MOUSE_SENSITIVITY = 0.1f;
        const float MIN_CAMERA_MOVE_SPEED = 0.1f;

        const MouseButton BTN_SELECT_MAP_OBJECT = MouseButton::LEFT;
        const Keycode KEY_MODIFIER = Keycode::LEFT_SHIFT;
        const Keycode KEY_DELETE_SELECTED_OBJECTS = Keycode::DEL;
        const Keycode KEY_COPY_SELECTED_OBJECTS = Keycode::C;
        const Keycode KEY_PASTE_COPIED_OBJECTS = Keycode::V;
        const Keycode KEY_UNDO = Keycode::Z;
        const Keycode KEY_REDO = Keycode::R;

        const MouseButton BTN_CAMERA_TOGGLE = MouseButton::RIGHT;
        const Keycode KEY_CAMERA_FORWARD = Keycode::W;
        const Keycode KEY_CAMERA_BACK = Keycode::S;
        const Keycode KEY_CAMERA_RIGHT = Keycode::A;
        const Keycode KEY_CAMERA_LEFT = Keycode::D;
        const Keycode KEY_CAMERA_UP = Keycode::SPACE;
        const Keycode KEY_CAMERA_DOWN = Keycode::LEFT_CONTROL;

        const int MAX_UNDO = 256;

        const char* QUIT_SAVE_FILE_NAME = "QUITSAVE";


        class ICommand {

        public:
                virtual void _Execute() = 0;
                virtual void _Undo() = 0;
                // ^ Never call these directly, use instead:
                //      - ExecuteCommand()
                //      - Undo()
                //      - Redo()

                virtual ~ICommand() = default;

        };


        IWindower* _windower;
        IRenderer* _renderer;
        IGUI* _gui;
        ISerializer* _serializer;

        std::vector<MapObjectRegistration> registered_map_objects;
        std::vector<MapObjectInstance> map_object_instances;
        std::vector<size_t> selected_map_objects_indices;
        std::vector<MapObjectInstance> map_object_instances_copy_buffer; // For Copy-Paste

        std::deque<std::unique_ptr<ICommand>> command_undo_stack;
        std::deque<std::unique_ptr<ICommand>> command_redo_stack;


        class PasteCommand : public ICommand {

        private:
                std::vector<MapObjectInstance>& _map_object_instances;
                std::vector<MapObjectInstance> _paste_data;
                size_t pasted_count = 0;


        public:
                PasteCommand(
                        std::vector<MapObjectInstance>& map_object_instances,
                        std::vector<MapObjectInstance> paste_data
                ) :
                        _map_object_instances(map_object_instances),
                        _paste_data(paste_data)
                {}

                void _Execute() override {
                        _map_object_instances.insert(
                                _map_object_instances.end(),
                                _paste_data.begin(),
                                _paste_data.end()
                        );
                        pasted_count = _paste_data.size();
                }

                void _Undo() override {
                        _map_object_instances.erase(
                                _map_object_instances.end() - pasted_count,
                                _map_object_instances.end()
                        );
                }

        };

        class DeleteCommand : public ICommand {

        private:
                std::vector<MapObjectInstance>& _map_object_instances;
                std::vector<size_t> _target_map_object_instances_indices;
                std::vector<MapObjectInstance> _deleted_map_object_instances;


        public:
                DeleteCommand(
                        std::vector<MapObjectInstance>& map_object_instances,
                        std::vector<size_t> target_map_object_instances_indices
                ) :
                        _map_object_instances(map_object_instances),
                        _target_map_object_instances_indices(target_map_object_instances_indices)
                {}

                void _Execute() override {
                        std::sort(
                                _target_map_object_instances_indices.rbegin(),
                                _target_map_object_instances_indices.rend()
                        );

                        _deleted_map_object_instances.clear();
                        for (size_t index : _target_map_object_instances_indices) {
                                _deleted_map_object_instances.push_back(_map_object_instances[index]);
                                _map_object_instances.erase(_map_object_instances.begin() + index);
                        }
                }

                void _Undo() override {
                        for (
                                size_t i = 0;
                                i < _target_map_object_instances_indices.size();
                                i++
                        ) {
                                _map_object_instances.insert(
                                        _map_object_instances.begin() + _target_map_object_instances_indices[i],
                                        _deleted_map_object_instances[_target_map_object_instances_indices.size()-1 - i]
                                );
                        }
                }

        };


        void LoadConfig(const std::string& config_path) {
                try {
                        std::ifstream in_data(config_path);
                        std::string _whitespace, path, default_extra_data;
                        while (true) {
                                if (!std::getline(in_data, _whitespace, '"')) {
                                        break; // EOF
                                }

                                if (!std::getline(in_data, path, '"')) {
                                        throw std::runtime_error(
                                                std::string("ERROR: Failed to find ending quote for \"path\" at index ") + std::to_string(in_data.tellg())
                                        );
                                }

                                if (!std::getline(in_data, _whitespace, '"')) {
                                        throw std::runtime_error(
                                                std::string("ERROR: Failed to find starting quote for \"default_extra_data\" at index ") + std::to_string(in_data.tellg())
                                        );
                                }

                                if (!std::getline(in_data, default_extra_data, '"')) {
                                        throw std::runtime_error(
                                                std::string("ERROR: Failed to find ending quote for \"default_extra_data\" at index ") + std::to_string(in_data.tellg())
                                        );
                                }

                                RegisterMapObject(path, default_extra_data);

                        }
                }
                catch (const std::exception& e) {
                        throw std::runtime_error(
                                std::string("ERROR: Failed to load config file from path: ") +
                                '"' + config_path + '"' +
                                "\n\t^ exception: " + e.what()
                        );
                }
        }

        void ExecuteCommand(std::unique_ptr<ICommand> command) {
                command->_Execute();

                command_undo_stack.push_back(std::move(command));
                if (command_undo_stack.size() >= MAX_UNDO)
                        command_undo_stack.pop_front();
                
                command_redo_stack.clear();
        }


public:
        Editor(
                IWindower* windower,
                IRenderer* renderer,
                IGUI* gui,
                ISerializer* serializer,
                const std::string& config_path = {}
        ) {
                _windower = windower;
                _renderer = renderer;
                _gui = gui;
                _serializer = serializer;

                if (!config_path.empty())
                        LoadConfig(config_path);
        }

        inline void RegisterMapObject(
                const std::string& path,
                const std::string& default_extra_data = {}
        ) override {
                std::string map_object_file_name = std::filesystem::path(path).stem().string();
                for (MapObjectRegistration map_object_registration : registered_map_objects) {
                                if (map_object_registration.name.compare(map_object_file_name) == 0) {
                                        throw std::runtime_error(
                                                std::string("ERROR: A MapObject ") +
                                                '"' + map_object_file_name + '"' +
                                                " is already registered!" +
                                                "\n\t^ attempted re-registration path: " +
                                                '"' + path + '"'
                                );
                        }
                }

                registered_map_objects.push_back(
                        MapObjectRegistration{
                                .name = map_object_file_name,
                                .model = _renderer->Load(path),
                                .default_extra_data = default_extra_data
                        }
                );
        }

        inline void Run() override {
                Load(QUIT_SAVE_FILE_NAME);

                // TODO
                std::cout << "TEST" << std::endl; // TEMP; TEST

                Save(QUIT_SAVE_FILE_NAME);
        }

        inline void InstantiateMapObject(
                const std::string& name, // <= filename in `path` in `RegisterMapObject()` (without extension)
                const std::array<float, 3>& pos,
                const std::array<float, 3>& rot,
                const std::array<float, 3>& scale,
                const std::string& extra_data = {}
        ) override {
                MapObjectRegistration* map_object_registration = nullptr;
                for (int i = 0; i < registered_map_objects.size(); i++) {
                        if (registered_map_objects[i].name.compare(name) == 0) {
                                map_object_registration = &registered_map_objects[i];
                        }
                }

                MapObjectInstance map_object_instance{
                        .name = map_object_registration->name,
                        .pos = pos,
                        .rot = rot,
                        .scale = scale
                };
                if (!extra_data.empty()) map_object_instance.extra_data = extra_data;
                else map_object_instance.extra_data = map_object_registration->default_extra_data;

                map_object_instances.push_back(map_object_instance);
                selected_map_objects_indices.clear();
                selected_map_objects_indices.push_back(map_object_instances.size() - 1);
        }

        inline void Save(const std::string& path) override {
                try {
                        std::ofstream out_file(path);
                        out_file << _serializer->Serialize(map_object_instances);
                        out_file.close();
                }
                catch (const std::exception& e) {
                        std::cout << "ERROR: Failed to save map to path: " <<
                        '"' << path << '"' <<
                        "\n\t^ exception: " << e.what() << std::endl;
                }
        }

        inline void Load(const std::string& path) override {
                try {
                        std::ifstream in_file(path);
                        in_file.seekg(0, std::ios::end);
                        size_t serialized_data_size = in_file.tellg();
                        std::string serialized_data(serialized_data_size, '\0');
                        in_file.seekg(0);
                        in_file.read(&serialized_data[0], serialized_data_size);

                        map_object_instances = _serializer->DeserializeMapObjects(serialized_data);
                }
                catch (const std::exception& e) {
                        std::cout << "ERROR: Failed to load map from path: " <<
                        '"' << path << '"' <<
                        "\n\t^ exception: " << e.what() << std::endl;
                }
        }

        void Undo() {
                if (!command_undo_stack.empty()) {
                        std::unique_ptr<ICommand> command = std::move(command_undo_stack.back());
                        command_undo_stack.pop_back();
                        command->_Undo();
                        command_redo_stack.push_back(std::move(command));
                }
        }

        void Redo() {
                if (!command_redo_stack.empty()) {
                        std::unique_ptr<ICommand> command = std::move(command_redo_stack.back());
                        command_redo_stack.pop_back();
                        command->_Execute();

                        command_undo_stack.push_back(std::move(command));
                        if (command_undo_stack.size() >= MAX_UNDO)
                                command_undo_stack.pop_front();
                }
        }
        
        void Paste() {
                ExecuteCommand(
                        std::make_unique<PasteCommand>(
                                map_object_instances,
                                map_object_instances_copy_buffer
                        )
                );
        }

        void Delete(std::vector<MapObjectInstance>& map_object_instances) {
                ExecuteCommand(
                        std::make_unique<DeleteCommand>(
                                map_object_instances,
                                selected_map_objects_indices
                        )
                );
        }

        void Clear() {
                std::vector<size_t> all_map_object_instances_indices(map_object_instances.size());
                std::iota(all_map_object_instances_indices.begin(), all_map_object_instances_indices.end(), 0);
                ExecuteCommand(
                        std::make_unique<DeleteCommand>(
                                map_object_instances,
                                all_map_object_instances_indices
                        )
                );
        }

};

#endif // _OPENMAP_EDITOR_HPP