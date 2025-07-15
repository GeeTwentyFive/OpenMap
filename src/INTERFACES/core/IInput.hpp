#ifndef _OPENMAP_IINPUT_HPP
#define _OPENMAP_IINPUT_HPP

#include <tuple>


class IInput {

public:
        enum Keycode {
                // TODO
        };

        virtual void Map(
                Keycode keycode,
                void(*callback)(void)
        ) = 0;

        virtual std::tuple<int, int> GetCursorPos() = 0;

        virtual ~IInput() = default;

};

#endif // _OPENMAP_IINPUT_HPP