#ifndef _OPENMAP_GLFWWINDOWER_HPP
#define _OPENMAP_GLFWWINDOWER_HPP

#include <src/INTERFACES/core/IWindower.hpp>


class GLFWWindower : public IWindower {

public:
        GLFWWindower() {
                // TODO
        }

        inline bool WindowShouldClose() override {
                // TODO
        }

        inline void UpdateInputState(InputState& input_state) override {
                // TODO
        }

};

#endif // _OPENMAP_GLFWWINDOWER_HPP