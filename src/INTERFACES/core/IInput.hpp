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
        );

        virtual std::tuple<int, int> GetCursorPos();

};

#endif // _OPENMAP_IINPUT_HPP