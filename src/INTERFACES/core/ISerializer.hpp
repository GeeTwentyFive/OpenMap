#ifndef _OPENMAP_ISERIALIZER_HPP
#define _OPENMAP_ISERIALIZER_HPP

#include <vector>
#include <src/INTERFACES/editor/IEditor.hpp>

class ISerializer {

public:
	virtual std::string Serialize(const std::vector<IEditor::MapObjectInstance>& map_object_instances) = 0;

	virtual std::vector<IEditor::MapObjectInstance> DeserializeMapObjects(const std::string& data) = 0;

};

#endif // _OPENMAP_ISERIALIZER_HPP