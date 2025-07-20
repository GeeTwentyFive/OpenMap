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
                std::array<int, 2> size,
                std::vector<Button> buttons
        ) override {
                for (int i = 0; i < buttons.size(); i++) {
                        int top = size[1] * i; // TEMP: Hacky solution; TODO: Refactor
                        if (
                                GuiButton(
                                        Rectangle{
                                                (float)top_left[0],
                                                (float)top,
                                                (float)size[0],
                                                (float)size[1]
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
                std::array<int, 2> size,
                std::vector<ImageButton> buttons
        ) override {
                // TODO
        }

        inline void DrawInputBoxes(
                std::array<int, 2> top_left,
                std::array<int, 2> size,
                std::map<std::string, std::string*> fields
        ) override {
                // TODO
        }

        inline bool ShowConfirmBox(std::string text) override {
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

        inline std::string ShowTextInputBox(std::string prompt) override {
                int screen_width = GetScreenWidth();
                int screen_height = GetScreenHeight();
                int result = -1;
                char text[1024];
                while (result == -1) {
                        BeginDrawing(); // "Temporary" hacky solution; TODO: Refactor
                        result = GuiTextInputBox(
                                Rectangle {
                                        (float)screen_width / 4,
                                        (float)screen_height / 4,
                                        (float)screen_width / 2,
                                        (float)screen_height / 2
                                },
                                "",
                                prompt.c_str(),
                                "Confirm;Cancel",
                                text,
                                sizeof(text),
                                NULL
                        );
                        EndDrawing(); // "Temporary" hacky solution; TODO: Refactor
                }
                if (result != 1) return std::string();
                return std::string(text);
        }

};

#endif // _OPENMAP_RAYGUI_GUI_HPP