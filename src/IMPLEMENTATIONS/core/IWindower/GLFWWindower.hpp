#ifndef _OPENMAP_GLFWWINDOWER_HPP
#define _OPENMAP_GLFWWINDOWER_HPP

#include <src/INTERFACES/core/IWindower.hpp>

#include <stdexcept>
#include <string>
#include <libs/glad/glad.h>
#include <libs/glfw/include/GLFW/glfw3.h>


class GLFWWindower : public IWindower {

private:
        GLFWwindow* window;


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

                if (glfwRawMouseMotionSupported())
                        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        }

        inline bool WindowShouldClose() override {
                return glfwWindowShouldClose(window);
        }

        inline void SwapBuffers() override {
                glfwSwapBuffers(window);
        }

        inline void UpdateInputState(InputState& input_state) override {
                // To access input_state from within callback lambdas:
                glfwSetWindowUserPointer(window, &input_state);

                // TODO: Input callbacks

                glfwPollEvents();
        }

        inline double GetTime() override {
                return glfwGetTime();
        }

        ~GLFWWindower() {
                glfwTerminate();
        }

};

#endif // _OPENMAP_GLFWWINDOWER_HPP