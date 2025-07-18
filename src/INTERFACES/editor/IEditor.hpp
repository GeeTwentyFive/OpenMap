#ifndef _OPENMAP_IEDITOR_HPP
#define _OPENMAP_IEDITOR_HPP

#include <src/INTERFACES/core/IDraw.hpp>
#include <src/INTERFACES/core/IInput.hpp>
#include <src/INTERFACES/core/ILua.hpp>


class IEditor {

public:
        virtual int Run(
                IDraw&& drawer,
                IInput&& input,
                ILua&& lua
        ) = 0;

        virtual ~IEditor() = default;

};

#endif // _OPENMAP_IEDITOR_HPP