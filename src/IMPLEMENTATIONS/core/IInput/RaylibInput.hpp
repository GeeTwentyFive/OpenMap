#ifndef _OPENMAP_RAYLIB_INPUT_HPP
#define _OPENMAP_RAYLIB_INPUT_HPP

#include <src/INTERFACES/core/IInput.hpp>
#include <tuple>

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <libs/raylib/include/raylib.h>


class RaylibInput: public IInput {

private:
	const std::unordered_map<Keycode, int> KEYCODE_MAP {
		{Keycode::APOSTROPHE, KEY_APOSTROPHE},
		{Keycode::COMMA, KEY_COMMA},
		{Keycode::MINUS, KEY_MINUS},
		{Keycode::PERIOD, KEY_PERIOD},
		{Keycode::SLASH, KEY_SLASH},
		{Keycode::ZERO, KEY_ZERO},
		{Keycode::ONE, KEY_ONE},
		{Keycode::TWO, KEY_TWO},
		{Keycode::THREE, KEY_THREE},
		{Keycode::FOUR, KEY_FOUR},
		{Keycode::FIVE, KEY_FIVE},
		{Keycode::SIX, KEY_SIX},
		{Keycode::SEVEN, KEY_SEVEN},
		{Keycode::EIGHT, KEY_EIGHT},
		{Keycode::NINE, KEY_NINE},
		{Keycode::SEMICOLON, KEY_SEMICOLON},
		{Keycode::EQUAL, KEY_EQUAL},
		{Keycode::A, KEY_A},
		{Keycode::B, KEY_B},
		{Keycode::C, KEY_C},
		{Keycode::D, KEY_D},
		{Keycode::E, KEY_E},
		{Keycode::F, KEY_F},
		{Keycode::G, KEY_G},
		{Keycode::H, KEY_H},
		{Keycode::I, KEY_I},
		{Keycode::J, KEY_J},
		{Keycode::K, KEY_K},
		{Keycode::L, KEY_L},
		{Keycode::M, KEY_M},
		{Keycode::N, KEY_N},
		{Keycode::O, KEY_O},
		{Keycode::P, KEY_P},
		{Keycode::Q, KEY_Q},
		{Keycode::R, KEY_R},
		{Keycode::S, KEY_S},
		{Keycode::T, KEY_T},
		{Keycode::U, KEY_U},
		{Keycode::V, KEY_V},
		{Keycode::W, KEY_W},
		{Keycode::X, KEY_X},
		{Keycode::Y, KEY_Y},
		{Keycode::Z, KEY_Z},
		{Keycode::LEFT_BRACKET, KEY_LEFT_BRACKET},
		{Keycode::BACKSLASH, KEY_BACKSLASH},
		{Keycode::RIGHT_BRACKET, KEY_RIGHT_BRACKET},
		{Keycode::GRAVE, KEY_GRAVE},
		{Keycode::SPACE, KEY_SPACE},
		{Keycode::ESCAPE, KEY_ESCAPE},
		{Keycode::ENTER, KEY_ENTER},
		{Keycode::TAB, KEY_TAB},
		{Keycode::BACKSPACE, KEY_BACKSPACE},
		{Keycode::INSERT, KEY_INSERT},
		{Keycode::DELETE, KEY_DELETE},
		{Keycode::RIGHT, KEY_RIGHT},
		{Keycode::LEFT, KEY_LEFT},
		{Keycode::DOWN, KEY_DOWN},
		{Keycode::UP, KEY_UP},
		{Keycode::PAGE_UP, KEY_PAGE_UP},
		{Keycode::PAGE_DOWN, KEY_PAGE_DOWN},
		{Keycode::HOME, KEY_HOME},
		{Keycode::END, KEY_END},
		{Keycode::CAPS_LOCK, KEY_CAPS_LOCK},
		{Keycode::SCROLL_LOCK, KEY_SCROLL_LOCK},
		{Keycode::NUM_LOCK, KEY_NUM_LOCK},
		{Keycode::PRINT_SCREEN, KEY_PRINT_SCREEN},
		{Keycode::PAUSE, KEY_PAUSE},
		{Keycode::F1, KEY_F1},
		{Keycode::F2, KEY_F2},
		{Keycode::F3, KEY_F3},
		{Keycode::F4, KEY_F4},
		{Keycode::F5, KEY_F5},
		{Keycode::F6, KEY_F6},
		{Keycode::F7, KEY_F7},
		{Keycode::F8, KEY_F8},
		{Keycode::F9, KEY_F9},
		{Keycode::F10, KEY_F10},
		{Keycode::F11, KEY_F11},
		{Keycode::F12, KEY_F12},
		{Keycode::LEFT_SHIFT, KEY_LEFT_SHIFT},
		{Keycode::LEFT_CONTROL, KEY_LEFT_CONTROL},
		{Keycode::LEFT_ALT, KEY_LEFT_ALT},
		{Keycode::LEFT_SUPER, KEY_LEFT_SUPER},
		{Keycode::RIGHT_SHIFT, KEY_RIGHT_SHIFT},
		{Keycode::RIGHT_CONTROL, KEY_RIGHT_CONTROL},
		{Keycode::RIGHT_ALT, KEY_RIGHT_ALT},
		{Keycode::RIGHT_SUPER, KEY_RIGHT_SUPER},
		{Keycode::KB_MENU, KEY_KB_MENU},
		// Keypad keys
		{Keycode::KP_0, KEY_KP_0},
		{Keycode::KP_1, KEY_KP_1},
		{Keycode::KP_2, KEY_KP_2},
		{Keycode::KP_3, KEY_KP_3},
		{Keycode::KP_4, KEY_KP_4},
		{Keycode::KP_5, KEY_KP_5},
		{Keycode::KP_6, KEY_KP_6},
		{Keycode::KP_7, KEY_KP_7},
		{Keycode::KP_8, KEY_KP_8},
		{Keycode::KP_9, KEY_KP_9},
		{Keycode::KP_DECIMAL, KEY_KP_DECIMAL},
		{Keycode::KP_DIVIDE, KEY_KP_DIVIDE},
		{Keycode::KP_MULTIPLY, KEY_KP_MULTIPLY},
		{Keycode::KP_SUBTRACT, KEY_KP_SUBTRACT},
		{Keycode::KP_ADD, KEY_KP_ADD},
		{Keycode::KP_ENTER, KEY_KP_ENTER},
		{Keycode::KP_EQUAL, KEY_KP_EQUAL},
		// Mouse buttons (PUT ONLY MOUSE BUTTONS PAST THIS POINT)
		{Keycode::MB_LEFT, MOUSE_BUTTON_LEFT},
		{Keycode::MB_RIGHT, MOUSE_BUTTON_RIGHT},
		{Keycode::MB_MIDDLE, MOUSE_BUTTON_MIDDLE}
	};

	std::unordered_map<
		Keycode,
		std::vector< std::function<void()> >
	> subscribers;


public:
        inline void Map(
                Keycode keycode,
                std::function<void()> callback
        ) override {
		subscribers[keycode].push_back(callback);
	}

	inline void Update() override {
		for (
			const std::pair<
				Keycode,
				std::vector< std::function<void()> >
			> &kv : subscribers
		) {
			bool notify = false;

			if (kv.first >= Keycode::MB_LEFT) {
				if (IsMouseButtonPressed(KEYCODE_MAP.at(kv.first))) {
					notify = true;
				}
			}
			else {
				if (IsKeyPressed(KEYCODE_MAP.at(kv.first))) {
					notify = true;
				}
			}

			if (notify) {
				for (
					const std::function<void()> &callback : kv.second
				) {
					callback();
				}
			}
		}
	}

        inline std::tuple<int, int> GetCursorPos() override {
		Vector2 mouse_pos = GetMousePosition();
		return std::tuple<int, int> (
			mouse_pos.x,
			mouse_pos.y
		);
	}
        
        inline std::tuple<int, int> GetMouseDelta() override {
		Vector2 mouse_delta = ::GetMouseDelta();
		return std::tuple<int, int> (
			mouse_delta.x,
			mouse_delta.y
		);
	}

	inline std::tuple<int, int> GetMouseWheelDelta() override {
		Vector2 mouse_wheel_delta = GetMouseWheelMoveV();
		return std::tuple<int, int> (
			mouse_wheel_delta.x,
			mouse_wheel_delta.y
		);
	}

	inline void LockCursor() override {
		DisableCursor();
	}
	inline void UnlockCursor() override {
		EnableCursor();
	}

};

#endif // _OPENMAP_RAYLIB_INPUT_HPP
