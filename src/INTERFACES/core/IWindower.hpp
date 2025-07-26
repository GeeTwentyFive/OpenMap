#ifndef _OPENMAP_IWINDOWER_HPP
#define _OPENMAP_IWINDOWER_HPP

#include <utility>
#include <unordered_map>


enum class OpenMap_Keycode {
        UNKNOWN = -1,

        APOSTROPHE,
        COMMA,
        MINUS,
        PERIOD,
        SLASH,
        ZERO,
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        SEMICOLON,
        EQUAL,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        LEFT_BRACKET,
        BACKSLASH,
        RIGHT_BRACKET,
        GRAVE,
        SPACE,
        ESCAPE,
        ENTER,
        TAB,
        BACKSPACE,
        INSERT,
        DEL,
        RIGHT,
        LEFT,
        DOWN,
        UP,
        PAGE_UP,
        PAGE_DOWN,
        HOME,
        END,
        CAPS_LOCK,
        SCROLL_LOCK,
        NUM_LOCK,
        PRINT_SCREEN,
        PAUSE,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        LEFT_SHIFT,
        LEFT_CONTROL,
        LEFT_ALT,
        LEFT_SUPER,
        RIGHT_SHIFT,
        RIGHT_CONTROL,
        RIGHT_ALT,
        RIGHT_SUPER,
        KB_MENU,
        // Keypad keys
        KP_0,
        KP_1,
        KP_2,
        KP_3,
        KP_4,
        KP_5,
        KP_6,
        KP_7,
        KP_8,
        KP_9,
        KP_DECIMAL,
        KP_DIVIDE,
        KP_MULTIPLY,
        KP_SUBTRACT,
        KP_ADD,
        KP_ENTER,
        KP_EQUAL,

        LAST
};

enum class OpenMap_MouseButton {
        UNKNOWN = -1,
        LEFT,
        RIGHT,
        MIDDLE
};

struct InputState {
        std::unordered_map<OpenMap_Keycode, bool> keys_down;
        std::unordered_map<OpenMap_Keycode, bool> keys_pressed;
        std::unordered_map<OpenMap_Keycode, bool> keys_released;

        std::pair<int, int> cursor_position;
        std::pair<int, int> last_cursor_position;
        std::pair<double, double> mouse_wheel_delta;
        std::unordered_map<OpenMap_MouseButton, bool> mouse_buttons_down;
        std::unordered_map<OpenMap_MouseButton, bool> mouse_buttons_pressed;
        std::unordered_map<OpenMap_MouseButton, bool> mouse_buttons_released;
};


class IWindower {

public:
        virtual bool WindowShouldClose() = 0;

        virtual void SwapBuffers() = 0;

        virtual double GetTime() = 0;

        virtual void UpdateInputState(InputState& input_state) = 0;

        virtual void LockCursor() = 0;
        virtual void UnlockCursor() = 0;

        virtual int GetMonitorRefreshRate() = 0;

        virtual void SleepUntilFPS(
                double target_fps,
                double frame_start_time
        ) = 0;

        virtual ~IWindower() = default;

};

#endif // _OPENMAP_IWINDOWER_HPP