#ifndef _OPENMAP_WINDOWER_HPP
#define _OPENMAP_WINDOWER_HPP

#include <src/INTERFACES/core/IWindower.hpp>

#include <thread>
#include <chrono>
#include <libs/raylib/include/raylib.h>

class Windower : public IWindower {

private:
	const std::unordered_map<OpenMap_Keycode, int> KEYCODE_MAP {
		{OpenMap_Keycode::APOSTROPHE, KEY_APOSTROPHE},
		{OpenMap_Keycode::COMMA, KEY_COMMA},
		{OpenMap_Keycode::MINUS, KEY_MINUS},
		{OpenMap_Keycode::PERIOD, KEY_PERIOD},
		{OpenMap_Keycode::SLASH, KEY_SLASH},
		{OpenMap_Keycode::ZERO, KEY_ZERO},
		{OpenMap_Keycode::ONE, KEY_ONE},
		{OpenMap_Keycode::TWO, KEY_TWO},
		{OpenMap_Keycode::THREE, KEY_THREE},
		{OpenMap_Keycode::FOUR, KEY_FOUR},
		{OpenMap_Keycode::FIVE, KEY_FIVE},
		{OpenMap_Keycode::SIX, KEY_SIX},
		{OpenMap_Keycode::SEVEN, KEY_SEVEN},
		{OpenMap_Keycode::EIGHT, KEY_EIGHT},
		{OpenMap_Keycode::NINE, KEY_NINE},
		{OpenMap_Keycode::SEMICOLON, KEY_SEMICOLON},
		{OpenMap_Keycode::EQUAL, KEY_EQUAL},
		{OpenMap_Keycode::A, KEY_A},
		{OpenMap_Keycode::B, KEY_B},
		{OpenMap_Keycode::C, KEY_C},
		{OpenMap_Keycode::D, KEY_D},
		{OpenMap_Keycode::E, KEY_E},
		{OpenMap_Keycode::F, KEY_F},
		{OpenMap_Keycode::G, KEY_G},
		{OpenMap_Keycode::H, KEY_H},
		{OpenMap_Keycode::I, KEY_I},
		{OpenMap_Keycode::J, KEY_J},
		{OpenMap_Keycode::K, KEY_K},
		{OpenMap_Keycode::L, KEY_L},
		{OpenMap_Keycode::M, KEY_M},
		{OpenMap_Keycode::N, KEY_N},
		{OpenMap_Keycode::O, KEY_O},
		{OpenMap_Keycode::P, KEY_P},
		{OpenMap_Keycode::Q, KEY_Q},
		{OpenMap_Keycode::R, KEY_R},
		{OpenMap_Keycode::S, KEY_S},
		{OpenMap_Keycode::T, KEY_T},
		{OpenMap_Keycode::U, KEY_U},
		{OpenMap_Keycode::V, KEY_V},
		{OpenMap_Keycode::W, KEY_W},
		{OpenMap_Keycode::X, KEY_X},
		{OpenMap_Keycode::Y, KEY_Y},
		{OpenMap_Keycode::Z, KEY_Z},
		{OpenMap_Keycode::LEFT_BRACKET, KEY_LEFT_BRACKET},
		{OpenMap_Keycode::BACKSLASH, KEY_BACKSLASH},
		{OpenMap_Keycode::RIGHT_BRACKET, KEY_RIGHT_BRACKET},
		{OpenMap_Keycode::GRAVE, KEY_GRAVE},
		{OpenMap_Keycode::SPACE, KEY_SPACE},
		{OpenMap_Keycode::ESCAPE, KEY_ESCAPE},
		{OpenMap_Keycode::ENTER, KEY_ENTER},
		{OpenMap_Keycode::TAB, KEY_TAB},
		{OpenMap_Keycode::BACKSPACE, KEY_BACKSPACE},
		{OpenMap_Keycode::INSERT, KEY_INSERT},
		{OpenMap_Keycode::DEL, KEY_DELETE},
		{OpenMap_Keycode::RIGHT, KEY_RIGHT},
		{OpenMap_Keycode::LEFT, KEY_LEFT},
		{OpenMap_Keycode::DOWN, KEY_DOWN},
		{OpenMap_Keycode::UP, KEY_UP},
		{OpenMap_Keycode::PAGE_UP, KEY_PAGE_UP},
		{OpenMap_Keycode::PAGE_DOWN, KEY_PAGE_DOWN},
		{OpenMap_Keycode::HOME, KEY_HOME},
		{OpenMap_Keycode::END, KEY_END},
		{OpenMap_Keycode::CAPS_LOCK, KEY_CAPS_LOCK},
		{OpenMap_Keycode::SCROLL_LOCK, KEY_SCROLL_LOCK},
		{OpenMap_Keycode::NUM_LOCK, KEY_NUM_LOCK},
		{OpenMap_Keycode::PRINT_SCREEN, KEY_PRINT_SCREEN},
		{OpenMap_Keycode::PAUSE, KEY_PAUSE},
		{OpenMap_Keycode::F1, KEY_F1},
		{OpenMap_Keycode::F2, KEY_F2},
		{OpenMap_Keycode::F3, KEY_F3},
		{OpenMap_Keycode::F4, KEY_F4},
		{OpenMap_Keycode::F5, KEY_F5},
		{OpenMap_Keycode::F6, KEY_F6},
		{OpenMap_Keycode::F7, KEY_F7},
		{OpenMap_Keycode::F8, KEY_F8},
		{OpenMap_Keycode::F9, KEY_F9},
		{OpenMap_Keycode::F10, KEY_F10},
		{OpenMap_Keycode::F11, KEY_F11},
		{OpenMap_Keycode::F12, KEY_F12},
		{OpenMap_Keycode::LEFT_SHIFT, KEY_LEFT_SHIFT},
		{OpenMap_Keycode::LEFT_CONTROL, KEY_LEFT_CONTROL},
		{OpenMap_Keycode::LEFT_ALT, KEY_LEFT_ALT},
		{OpenMap_Keycode::LEFT_SUPER, KEY_LEFT_SUPER},
		{OpenMap_Keycode::RIGHT_SHIFT, KEY_RIGHT_SHIFT},
		{OpenMap_Keycode::RIGHT_CONTROL, KEY_RIGHT_CONTROL},
		{OpenMap_Keycode::RIGHT_ALT, KEY_RIGHT_ALT},
		{OpenMap_Keycode::RIGHT_SUPER, KEY_RIGHT_SUPER},
		{OpenMap_Keycode::KB_MENU, KEY_KB_MENU},
		// Keypad keys
		{OpenMap_Keycode::KP_0, KEY_KP_0},
		{OpenMap_Keycode::KP_1, KEY_KP_1},
		{OpenMap_Keycode::KP_2, KEY_KP_2},
		{OpenMap_Keycode::KP_3, KEY_KP_3},
		{OpenMap_Keycode::KP_4, KEY_KP_4},
		{OpenMap_Keycode::KP_5, KEY_KP_5},
		{OpenMap_Keycode::KP_6, KEY_KP_6},
		{OpenMap_Keycode::KP_7, KEY_KP_7},
		{OpenMap_Keycode::KP_8, KEY_KP_8},
		{OpenMap_Keycode::KP_9, KEY_KP_9},
		{OpenMap_Keycode::KP_DECIMAL, KEY_KP_DECIMAL},
		{OpenMap_Keycode::KP_DIVIDE, KEY_KP_DIVIDE},
		{OpenMap_Keycode::KP_MULTIPLY, KEY_KP_MULTIPLY},
		{OpenMap_Keycode::KP_SUBTRACT, KEY_KP_SUBTRACT},
		{OpenMap_Keycode::KP_ADD, KEY_KP_ADD},
		{OpenMap_Keycode::KP_ENTER, KEY_KP_ENTER},
		{OpenMap_Keycode::KP_EQUAL, KEY_KP_EQUAL},
	};

	const std::unordered_map<OpenMap_MouseButton, int> MOUSE_BUTTON_MAP {
		{OpenMap_MouseButton::LEFT, MOUSE_BUTTON_LEFT},
		{OpenMap_MouseButton::RIGHT, MOUSE_BUTTON_RIGHT},
		{OpenMap_MouseButton::MIDDLE, MOUSE_BUTTON_MIDDLE}
	};


public:
        Windower() {

                SetConfigFlags(FLAG_MSAA_4X_HINT);

                int window_min_width = ::GetScreenWidth() / 2;
                int window_min_height = ::GetScreenHeight() / 2;

                InitWindow(
                        window_min_width,
                        window_min_height,
                        ""
                );

                SetWindowMinSize(window_min_width, window_min_height);

                SetTargetFPS(0);

                SetExitKey(0);

        }

        inline bool WindowShouldClose() override {
                return ::WindowShouldClose();
        }

        inline void SwapBuffers() override {
                ::EndDrawing();
        }

        inline double GetTime() override {
                return ::GetTime();
        }

        inline bool IsKeyPressed(OpenMap_Keycode keycode) override {
		return ::IsKeyPressed(
			KEYCODE_MAP.at(keycode)
		);
	}
	inline bool IsKeyDown(OpenMap_Keycode keycode) override {
		return ::IsKeyDown(
			KEYCODE_MAP.at(keycode)
		);
	}

	inline bool IsMouseButtonPressed(OpenMap_MouseButton mouse_button) override {
		return ::IsMouseButtonPressed(
			MOUSE_BUTTON_MAP.at(mouse_button)
		);
	}
	inline bool IsMouseButtonDown(OpenMap_MouseButton mouse_button) override {
		return ::IsMouseButtonDown(
			MOUSE_BUTTON_MAP.at(mouse_button)
		);
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
                ::DisableCursor();
        }
        inline void UnlockCursor() override {
                if (IsCursorHidden()) {
                        ::EnableCursor();
                }
        }

        inline int GetMonitorRefreshRate() override {
                return ::GetMonitorRefreshRate(::GetCurrentMonitor());
        }

        inline void SleepUntilFPS(
                double target_fps,
                double frame_start_time
        ) override {
                double remaining = (1.0/target_fps) - (GetTime() - frame_start_time);
                if (remaining > 1.0) {
                        std::this_thread::sleep_for(
                                std::chrono::duration<double>(remaining - 1.0)
                        );
                }
                while ((GetTime() - frame_start_time) < (1.0/target_fps)) {}
        }

        ~Windower() {
                ::CloseWindow();
        }

};

#endif // _OPENMAP_WINDOWER_HPP