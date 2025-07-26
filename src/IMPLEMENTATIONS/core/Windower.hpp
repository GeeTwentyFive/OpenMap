#ifndef _OPENMAP_WINDOWER_HPP
#define _OPENMAP_WINDOWER_HPP

#include <src/INTERFACES/core/IWindower.hpp>

#include <thread>
#include <chrono>
#include <libs/raylib-cpp/include/raylib-cpp.hpp>

class Windower : public IWindower {

private:
        raylib::Window window;


public:
        Windower() {

                int window_min_width = ::GetScreenWidth() / 2;
                int window_min_height = ::GetScreenHeight() / 2;

                window = raylib::Window(
                        window_min_width,
                        window_min_height,
                        "",
                        FLAG_MSAA_4X_HINT
                );

                window.SetMinSize(
                        raylib::Vector2(
                                window_min_width,
                                window_min_height
                        )
                );

                window.SetTargetFPS(0);

                SetExitKey(0);

        }

        inline bool WindowShouldClose() override {
                return window.ShouldClose();
        }

        inline void SwapBuffers() override {
                window.EndDrawing();
        }

        inline double GetTime() override {
                return window.GetTime();
        }

        inline void UpdateInputState(InputState& input_state) override {
                // TODO
        }

        inline void LockCursor() override {
                ::DisableCursor();
        }
        inline void UnlockCursor() override {
                if (window.IsCursorHidden()) {
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
                window.Close();
        }

};

#endif // _OPENMAP_WINDOWER_HPP