#ifndef _OPENMAP_OPENGLRENDERER_HPP
#define _OPENMAP_OPENGLRENDERER_HPP

#include <src/INTERFACES/core/IRenderer.hpp>

#include <filesystem>
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <libs/tinygltf/tiny_gltf.h>
#include <src/INTERFACES/editor/IEditor.hpp>


class OpenGLRenderer : public IRenderer {

private:
        // TODO:
        // - Draw map objects
        // - Draw bounding boxes around selected map objects


public:
        inline Model* Load(const std::string& path) override {

                tinygltf::Model model;
                try {
                        std::string file_extension = std::filesystem::path(path).extension();
                        
                        tinygltf::TinyGLTF loader;
                        std::string err, warn;
                        bool result;
                        if (file_extension == ".gltf") {
                                result = loader.LoadASCIIFromFile(&model, &err, &warn, path);
                        }
                        else if (file_extension == ".glb") {
                                result = loader.LoadBinaryFromFile(&model, &err, &warn, path);
                        }
                        else {
                                throw std::runtime_error(
                                        std::string("ERROR: Unsupported 3D model file type: ") +
                                        '"' + path + '"'
                                );
                        }

                        if (!warn.empty()) {
                                std::cout << "TinyGLTF warning: " << warn <<
                                        "\n\t^ for file: " << path << std::endl;
                        }

                        if (!err.empty()) {
                                std::cout << "TinyGLTF error: " << err <<
                                        "\n\t^ for file: " << path << std::endl;
                        }

                        if (!result) {
                                throw std::runtime_error(
                                        std::string("ERROR: TinyGLTF failed to parse 3D model at path: ") +
                                        '"' + path + '"'
                                );
                        }
                }
                catch (const std::exception& e) {
                        throw std::runtime_error(
                                std::string("ERROR: Failed to load 3D model at path: ") +
                                '"' + path + '"' +
                                "\n\t^ exception: " + e.what()
                        );
                }

                // TODO

        }

        inline void Clear() override {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        inline void Render(
                const std::vector<MapObjectInstance>& map_object_instances,
                const std::vector<size_t>& selected_map_objects_indices
        ) override {
                // TODO
        }

};

#endif // _OPENMAP_OPENGLRENDERER_HPP