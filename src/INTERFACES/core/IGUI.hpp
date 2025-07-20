#ifndef _OPENMAP_IGUI_HPP
#define _OPENMAP_IGUI_HPP

#include <array>
#include <string>
#include <functional>
#include <vector>
#include <map>

class IGUI {

public:
        struct Button {
                std::string label;
                std::function<void(std::string)> callback;
        };

        typedef struct Image;
        struct ImageButton{
                Image* image;
                std::string label;
                std::function<void(std::string)> callback;
        };


        virtual void DrawButtonsBox(
                std::array<int, 2> top_left,
                std::array<int, 2> bottom_right,
                std::vector<Button> buttons
        ) = 0;

        virtual Image* LoadImage(std::string path) = 0;

        virtual void DrawImageButtonsList(
                std::array<int, 2> top_left,
                std::array<int, 2> bottom_right,
                std::vector<ImageButton> buttons
        ) = 0;

        virtual void DrawInputBoxes(
                std::array<int, 2> top_left,
                std::array<int, 2> bottom_right,
                std::map<std::string, std::string*> fields
        ) = 0;

        virtual ~IGUI() = default;

};

#endif // _OPENMAP_IGUI_HPP