#ifndef _OPENMAP_SERIALIZER_HPP
#define _OPENMAP_SERIALIZER_HPP

#include <src/INTERFACES/core/ISerializer.hpp>

#include <stdexcept>
#include <libs/nlohmann/json.hpp>

class Serializer : public ISerializer {

        inline std::string Serialize(const std::vector<MapObjectInstance>& map_object_instances) override {
                nlohmann::json j_array = nlohmann::json::array();
                for (MapObjectInstance map_object : map_object_instances) {
                        nlohmann::json j;
                        j["path"] = map_object.path;
                        j["pos"] = map_object.pos;
                        j["rot"] = map_object.rot;
                        j["scale"] = map_object.scale;
                        j["extra_data"] = map_object.extra_data;

                        j_array.push_back(j);
                }

                return j_array.dump(4);
        }

        inline std::vector<MapObjectInstance> DeserializeMapObjects(const std::string& data) override {
                nlohmann::json j = nlohmann::json::parse(data);

                std::vector<MapObjectInstance> map_object_instances;
                for (const auto& item : j) {
                        map_object_instances.push_back(
                                MapObjectInstance{
                                        .path = item.at("path").get<std::string>(),
                                        .pos = item.at("pos").get<std::array<float, 3>>(),
                                        .rot = item.at("rot").get<std::array<float, 3>>(),
                                        .scale = item.at("scale").get<std::array<float, 3>>(),
                                        .extra_data = item.at("extra_data").get<std::string>()
                                }
                        );
                }

                return map_object_instances;
        }

};

#endif // _OPENMAP_SERIALIZER_HPP