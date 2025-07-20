#ifndef _OPENMAP_RAYGUI_GUI_HPP
#define _OPENMAP_RAYGUI_GUI_HPP

#include <src/INTERFACES/core/IGUI.hpp>

#include <libs/raylib/include/raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <libs/raygui/raygui.h>


class RayguiGUI : public IGUI {

public:
        inline void DrawButtonsBox(
                std::array<int, 2> top_left,
                std::array<int, 2> bottom_right,
                std::vector<Button> buttons
        ) override {
                for (int i = 0; i < buttons.size(); i++) {
                        int top = bottom_right[1] * i; // TEMP: Hacky solution; TODO: Refactor
                        if (
                                GuiButton(
                                        Rectangle{
                                                (float)top_left[0],
                                                (float)top,
                                                (float)bottom_right[0],
                                                (float)bottom_right[1]
                                        },
                                        buttons[i].label.c_str()
                                )
                        ) {
                                buttons[i].callback(buttons[i].label);
                        }
                }
        }

        inline Image* LoadImage(std::string path) override {
                // TODO
        }

        inline void DrawImageButtonsList(
                std::array<int, 2> top_left,
                std::array<int, 2> bottom_right,
                std::vector<ImageButton> buttons
        ) override {
                // TODO
        }

        inline void DrawInputBoxes(
                std::array<int, 2> top_left,
                std::array<int, 2> bottom_right,
                std::map<std::string, std::string*> fields
        ) override {
                // TODO
        }

        bool ShowConfirmBox(std::string text) override {
                int screen_width = GetScreenWidth();
                int screen_height = GetScreenHeight();
                int result = -1;
                while (result == -1) {
                        BeginDrawing(); // "Temporary" hacky solution; TODO: Refactor
                        result = GuiMessageBox(
                                Rectangle {
                                        (float)screen_width / 4,
                                        (float)screen_height / 4,
                                        (float)screen_width / 2,
                                        (float)screen_height / 2
                                },
                                "",
                                text.c_str(),
                                "Yes;No"
                        );
                        EndDrawing(); // "Temporary" hacky solution; TODO: Refactor
                }
                if (result == 1) return true;
                else return false;
        }

};

#endif // _OPENMAP_RAYGUI_GUI_HPP