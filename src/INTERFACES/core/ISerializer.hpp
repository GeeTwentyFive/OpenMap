#ifndef _OPENMAP_ISERIALIZER_HPP
#define _OPENMAP_ISERIALIZER_HPP

#include <vector>
#include <src/INTERFACES/editor/IEditor.hpp>

class ISerializer {

public:
	virtual std::vector<unsigned char> Serialize(const IEditor::MapObjectInstance& m) = 0;

	virtual IEditor::MapObjectInstance DeserializeMapObject(const std::vector<unsigned char>& data) = 0;

};

#endif // _OPENMAP_ISERIALIZER_HPP