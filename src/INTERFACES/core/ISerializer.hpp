#ifndef _OPENMAP_ISERIALIZER_HPP
#define _OPENMAP_ISERIALIZER_HPP

#include <vector>
#include <src/INTERFACES/editor/IEditor.hpp>

class ISerializer {

public:
	virtual std::string Serialize(const std::vector<MapObjectInstance>& map_object_instances) = 0;

	virtual std::vector<MapObjectInstance> DeserializeMapObjects(const std::string& data) = 0;

};

#endif // _OPENMAP_ISERIALIZER_HPP