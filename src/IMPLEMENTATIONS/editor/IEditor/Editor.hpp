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
#include <src/INTERFACES/core/IWindower.hpp>
#include <src/INTERFACES/core/IRenderer.hpp>
#include <src/INTERFACES/core/IGUI.hpp>
#include <src/INTERFACES/core/ISerializer.hpp>


class Editor : public IEditor {

private:
        const char* QUIT_SAVE_FILE_NAME = "QUITSAVE";


        IWindower* _windower;
        IRenderer* _renderer;
        IGUI* _gui;
        ISerializer* _serializer;

        std::unordered_map<std::string, MapObjectRegistration> map_objects; // string is name
        std::vector<IEditor::MapObjectInstance> map_object_instances;
        std::vector<IEditor::MapObjectInstance*> selected_map_objects;


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

                LoadConfig(config_path);
        }

        inline void RegisterMapObject(
                const std::string& path,
                const std::string& default_extra_data = {}
        ) override {
                std::string map_object_file_name = std::filesystem::path(path).stem().string();
                if (map_objects.count(map_object_file_name) != 0) {
                        throw std::runtime_error(
                                std::string("ERROR: A MapObject ") +
                                '"' + map_object_file_name + '"' +
                                " is already registered!" +
                                "\n\t^ attempted re-registration path: " +
                                '"' + path + '"'
                        );
                }
                map_objects[map_object_file_name] = MapObjectRegistration{
                        .model = _renderer->Load(path),
                        .default_extra_data = default_extra_data
                };
        }

        inline void Run() override {
                Load(QUIT_SAVE_FILE_NAME);

                // TODO

                Save(QUIT_SAVE_FILE_NAME);
        }

        inline void InstantiateMapObject(
                const std::string& name, // <= filename in `path` in `RegisterMapObject()` (without extension)
                const std::array<float, 3>& pos,
                const std::array<float, 3>& rot,
                const std::array<float, 3>& scale,
                const std::string& extra_data = {}
        ) override {
                if (map_objects.count(name) == 0) {
                        throw std::runtime_error(
                                std::string("ERROR (AddMapObject): MapObject ") +
                                '"' + name + '"' +
                                " not found!"
                        );
                }

                MapObjectInstance map_object_instance{
                        .name = name,
                        .model = map_objects[name].model,
                        .pos = pos,
                        .rot = rot,
                        .scale = scale
                };
                if (!extra_data.empty()) map_object_instance.extra_data = extra_data;
                else map_object_instance.extra_data = map_objects[name].default_extra_data;

                map_object_instances.push_back(map_object_instance);
                selected_map_objects.clear();
                selected_map_objects.push_back(&map_object_instance);
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
                        "\n\t^ exception: " << e.what();
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
                        "\n\t^ exception: " << e.what();
                }
        }

        inline void Clear() override {
                map_object_instances.clear();
        }

};

#endif // _OPENMAP_EDITOR_HPP