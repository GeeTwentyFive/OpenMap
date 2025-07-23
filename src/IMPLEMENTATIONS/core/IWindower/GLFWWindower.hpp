#ifndef _OPENMAP_GLFWWINDOWER_HPP
#define _OPENMAP_GLFWWINDOWER_HPP

#include <src/INTERFACES/core/IWindower.hpp>

#include <stdexcept>
#include <string>
#include <thread>
#include <chrono>
#include <libs/glad/glad.h>
#include <libs/glfw/include/GLFW/glfw3.h>


class GLFWWindower : public IWindower {

private:
        GLFWwindow* window;


        static constexpr Keycode FromGLFWKey(int glfw_keycode) {
                switch (glfw_keycode) {
                        case GLFW_KEY_APOSTROPHE:    return Keycode::APOSTROPHE;
                        case GLFW_KEY_COMMA:         return Keycode::COMMA;
                        case GLFW_KEY_MINUS:         return Keycode::MINUS;
                        case GLFW_KEY_PERIOD:        return Keycode::PERIOD;
                        case GLFW_KEY_SLASH:         return Keycode::SLASH;
                        case GLFW_KEY_0:             return Keycode::ZERO;
                        case GLFW_KEY_1:             return Keycode::ONE;
                        case GLFW_KEY_2:             return Keycode::TWO;
                        case GLFW_KEY_3:             return Keycode::THREE;
                        case GLFW_KEY_4:             return Keycode::FOUR;
                        case GLFW_KEY_5:             return Keycode::FIVE;
                        case GLFW_KEY_6:             return Keycode::SIX;
                        case GLFW_KEY_7:             return Keycode::SEVEN;
                        case GLFW_KEY_8:             return Keycode::EIGHT;
                        case GLFW_KEY_9:             return Keycode::NINE;
                        case GLFW_KEY_SEMICOLON:     return Keycode::SEMICOLON;
                        case GLFW_KEY_EQUAL:         return Keycode::EQUAL;
                        case GLFW_KEY_A:             return Keycode::A;
                        case GLFW_KEY_B:             return Keycode::B;
                        case GLFW_KEY_C:             return Keycode::C;
                        case GLFW_KEY_D:             return Keycode::D;
                        case GLFW_KEY_E:             return Keycode::E;
                        case GLFW_KEY_F:             return Keycode::F;
                        case GLFW_KEY_G:             return Keycode::G;
                        case GLFW_KEY_H:             return Keycode::H;
                        case GLFW_KEY_I:             return Keycode::I;
                        case GLFW_KEY_J:             return Keycode::J;
                        case GLFW_KEY_K:             return Keycode::K;
                        case GLFW_KEY_L:             return Keycode::L;
                        case GLFW_KEY_M:             return Keycode::M;
                        case GLFW_KEY_N:             return Keycode::N;
                        case GLFW_KEY_O:             return Keycode::O;
                        case GLFW_KEY_P:             return Keycode::P;
                        case GLFW_KEY_Q:             return Keycode::Q;
                        case GLFW_KEY_R:             return Keycode::R;
                        case GLFW_KEY_S:             return Keycode::S;
                        case GLFW_KEY_T:             return Keycode::T;
                        case GLFW_KEY_U:             return Keycode::U;
                        case GLFW_KEY_V:             return Keycode::V;
                        case GLFW_KEY_W:             return Keycode::W;
                        case GLFW_KEY_X:             return Keycode::X;
                        case GLFW_KEY_Y:             return Keycode::Y;
                        case GLFW_KEY_Z:             return Keycode::Z;
                        case GLFW_KEY_LEFT_BRACKET:  return Keycode::LEFT_BRACKET;
                        case GLFW_KEY_BACKSLASH:     return Keycode::BACKSLASH;
                        case GLFW_KEY_RIGHT_BRACKET: return Keycode::RIGHT_BRACKET;
                        case GLFW_KEY_GRAVE_ACCENT:  return Keycode::GRAVE;
                        case GLFW_KEY_SPACE:         return Keycode::SPACE;
                        case GLFW_KEY_ESCAPE:        return Keycode::ESCAPE;
                        case GLFW_KEY_ENTER:         return Keycode::ENTER;
                        case GLFW_KEY_TAB:           return Keycode::TAB;
                        case GLFW_KEY_BACKSPACE:     return Keycode::BACKSPACE;
                        case GLFW_KEY_INSERT:        return Keycode::INSERT;
                        case GLFW_KEY_DELETE:        return Keycode::DEL;
                        case GLFW_KEY_RIGHT:         return Keycode::RIGHT;
                        case GLFW_KEY_LEFT:          return Keycode::LEFT;
                        case GLFW_KEY_DOWN:          return Keycode::DOWN;
                        case GLFW_KEY_UP:            return Keycode::UP;
                        case GLFW_KEY_PAGE_UP:       return Keycode::PAGE_UP;
                        case GLFW_KEY_PAGE_DOWN:     return Keycode::PAGE_DOWN;
                        case GLFW_KEY_HOME:          return Keycode::HOME;
                        case GLFW_KEY_END:           return Keycode::END;
                        case GLFW_KEY_CAPS_LOCK:     return Keycode::CAPS_LOCK;
                        case GLFW_KEY_SCROLL_LOCK:   return Keycode::SCROLL_LOCK;
                        case GLFW_KEY_NUM_LOCK:      return Keycode::NUM_LOCK;
                        case GLFW_KEY_PRINT_SCREEN:  return Keycode::PRINT_SCREEN;
                        case GLFW_KEY_PAUSE:         return Keycode::PAUSE;
                        case GLFW_KEY_F1:            return Keycode::F1;
                        case GLFW_KEY_F2:            return Keycode::F2;
                        case GLFW_KEY_F3:            return Keycode::F3;
                        case GLFW_KEY_F4:            return Keycode::F4;
                        case GLFW_KEY_F5:            return Keycode::F5;
                        case GLFW_KEY_F6:            return Keycode::F6;
                        case GLFW_KEY_F7:            return Keycode::F7;
                        case GLFW_KEY_F8:            return Keycode::F8;
                        case GLFW_KEY_F9:            return Keycode::F9;
                        case GLFW_KEY_F10:           return Keycode::F10;
                        case GLFW_KEY_F11:           return Keycode::F11;
                        case GLFW_KEY_F12:           return Keycode::F12;
                        case GLFW_KEY_LEFT_SHIFT:    return Keycode::LEFT_SHIFT;
                        case GLFW_KEY_LEFT_CONTROL:  return Keycode::LEFT_CONTROL;
                        case GLFW_KEY_LEFT_ALT:      return Keycode::LEFT_ALT;
                        case GLFW_KEY_LEFT_SUPER:    return Keycode::LEFT_SUPER;
                        case GLFW_KEY_RIGHT_SHIFT:   return Keycode::RIGHT_SHIFT;
                        case GLFW_KEY_RIGHT_CONTROL: return Keycode::RIGHT_CONTROL;
                        case GLFW_KEY_RIGHT_ALT:     return Keycode::RIGHT_ALT;
                        case GLFW_KEY_RIGHT_SUPER:   return Keycode::RIGHT_SUPER;
                        case GLFW_KEY_MENU:          return Keycode::KB_MENU;
                        case GLFW_KEY_KP_0:          return Keycode::KP_0;
                        case GLFW_KEY_KP_1:          return Keycode::KP_1;
                        case GLFW_KEY_KP_2:          return Keycode::KP_2;
                        case GLFW_KEY_KP_3:          return Keycode::KP_3;
                        case GLFW_KEY_KP_4:          return Keycode::KP_4;
                        case GLFW_KEY_KP_5:          return Keycode::KP_5;
                        case GLFW_KEY_KP_6:          return Keycode::KP_6;
                        case GLFW_KEY_KP_7:          return Keycode::KP_7;
                        case GLFW_KEY_KP_8:          return Keycode::KP_8;
                        case GLFW_KEY_KP_9:          return Keycode::KP_9;
                        case GLFW_KEY_KP_DECIMAL:    return Keycode::KP_DECIMAL;
                        case GLFW_KEY_KP_DIVIDE:     return Keycode::KP_DIVIDE;
                        case GLFW_KEY_KP_MULTIPLY:   return Keycode::KP_MULTIPLY;
                        case GLFW_KEY_KP_SUBTRACT:   return Keycode::KP_SUBTRACT;
                        case GLFW_KEY_KP_ADD:        return Keycode::KP_ADD;
                        case GLFW_KEY_KP_ENTER:      return Keycode::KP_ENTER;
                        case GLFW_KEY_KP_EQUAL:      return Keycode::KP_EQUAL;
                        default:                     return Keycode::UNKNOWN;
                }
        }

        static constexpr MouseButton FromGLFWMouseButton(int glfw_button) {
                switch (glfw_button) {
                        case GLFW_MOUSE_BUTTON_LEFT: return MouseButton::LEFT;
                        case GLFW_MOUSE_BUTTON_RIGHT: return MouseButton::RIGHT;
                        case GLFW_MOUSE_BUTTON_MIDDLE: return MouseButton::MIDDLE;
                        default: return MouseButton::UNKNOWN;
                }
        }


public:
        GLFWWindower() {

                glfwSetErrorCallback(
                        [](int error_code, const char* description) {
                                throw std::runtime_error(
                                        std::string("GLFW ERROR (") +
                                        std::to_string(error_code) + "): " +
                                        '"' + description + '"'
                                );
                        }
                );

                if (!glfwInit())
                        throw std::runtime_error("ERROR: Failed to initialize GLFW\n");

                const GLFWvidmode* video_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
                const int screen_width = video_mode->width;
                const int screen_height = video_mode->height;

                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                glfwWindowHint(GLFW_SAMPLES, 4);
                window = glfwCreateWindow(
                        screen_width/2,
                        screen_height/2,
                        "",
                        NULL,
                        NULL
                );

                glfwMakeContextCurrent(window);

                gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

                glfwSetFramebufferSizeCallback(
                        window,
                        [](GLFWwindow* window, int width, int height) {
                                glViewport(0, 0, width, height);
                        }
                );

                glfwSwapInterval(0);

                if (glfwRawMouseMotionSupported())
                        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        }

        inline bool WindowShouldClose() override {
                return glfwWindowShouldClose(window);
        }

        inline void SwapBuffers() override {
                glfwSwapBuffers(window);
        }

        inline double GetTime() override {
                return glfwGetTime();
        }

        inline void UpdateInputState(InputState& input_state) override {
                input_state.keys_pressed.clear();
                input_state.keys_released.clear();
                input_state.mouse_buttons_pressed.clear();
                input_state.mouse_buttons_released.clear();

                // To access input_state from within callback lambdas:
                glfwSetWindowUserPointer(window, &input_state);

                glfwSetKeyCallback(
                        window,
                        [](GLFWwindow* window, int key, int, int action, int) {
                                InputState* input_state = (InputState*)glfwGetWindowUserPointer(window);
                                Keycode _key = FromGLFWKey(key);
                                input_state->keys_down[_key] = (action == GLFW_PRESS);
                                input_state->keys_pressed[_key] = (action == GLFW_PRESS);
                                input_state->keys_released[_key] = (action == GLFW_RELEASE);
                        }
                );

                glfwSetCursorPosCallback(
                        window,
                        [](GLFWwindow* window, double x_pos, double y_pos) {
                                InputState* input_state = (InputState*)glfwGetWindowUserPointer(window);
                                input_state->last_cursor_position = input_state->cursor_position;
                                input_state->cursor_position = {x_pos, y_pos};
                        }
                );

                glfwSetMouseButtonCallback(
                        window,
                        [](GLFWwindow* window, int button, int action, int) {
                                InputState* input_state = (InputState*)glfwGetWindowUserPointer(window);
                                MouseButton _mouse_button = FromGLFWMouseButton(button);
                                input_state->mouse_buttons_down[_mouse_button] = (action == GLFW_PRESS);
                                input_state->mouse_buttons_pressed[_mouse_button] = (action == GLFW_PRESS);
                                input_state->mouse_buttons_released[_mouse_button] = (action == GLFW_RELEASE);
                        }
                );

                glfwSetScrollCallback(
                        window,
                        [](GLFWwindow* window, double x_offset, double y_offset) {
                                InputState* input_state = (InputState*)glfwGetWindowUserPointer(window);
                                input_state->mouse_wheel_delta = {x_offset, y_offset};
                        }
                );

                glfwPollEvents();
        }

        inline void LockCursor() override {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        inline void UnlockCursor() override {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        inline int GetMonitorRefreshRate() override {
                return glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate;
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

        ~GLFWWindower() {
                glfwTerminate();
        }

};

#endif // _OPENMAP_GLFWWINDOWER_HPP