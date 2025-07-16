#ifndef _OPENMAP_RAYLIB_INPUT_HPP
#define _OPENMAP_RAYLIB_INPUT_HPP

#include <src/INTERFACES/core/IInput.hpp>

#include <tuple>

#include <libs/raylib/include/raylib.h>


class RaylibInput: IInput {

public:
        virtual void Map(
                Keycode keycode,
                void(*callback)(void)
        ) {
		// TODO
	}

        virtual std::tuple<int, int> GetCursorPos() {
		Vector2 mouse_pos = GetMousePosition();
		return std::tuple<int, int> (
			mouse_pos.x,
			mouse_pos.y
		);
	}
        
        virtual std::tuple<int, int> GetMouseDelta() {
		Vector2 mouse_delta = ::GetMouseDelta();
		return std::tuple<int, int> (
			mouse_delta.x,
			mouse_delta.y
		);
	}

};

#endif // _OPENMAP_RAYLIB_INPUT_HPP
