#ifndef _OPENMAP_IEDITOR_HPP
#define _OPENMAP_IEDITOR_HPP

#include <src/INTERFACES/core/IDraw.hpp>
// TODO


class IEditor {

public:
        virtual int Run(IDraw&& drawer) = 0;

        virtual ~IEditor() = default;

};

#endif // _OPENMAP_IEDITOR_HPP