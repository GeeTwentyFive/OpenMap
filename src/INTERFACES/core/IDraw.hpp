#ifndef _OPENMAP_IDRAW_HPP
#define _OPENMAP_IDRAW_HPP


class IDraw {

public:
        typedef struct Model;
        typedef struct Sprite;


        virtual Model* LoadModel(const char* target_file_path) = 0;
        virtual void UnloadModel(Model* model) = 0;

        virtual Sprite* LoadSprite(const char* target_file_path) = 0;
        virtual void UnloadSprite(Sprite* sprite) = 0;


        //virtual void ToggleCameraProjection(); // TODO


        virtual bool WindowShouldClose() = 0;

        virtual void BeginDrawing(int clear_color = 0xffffffff) = 0;

        virtual void Draw(
                Model* model,
                float position[3],
                float rotation[3],
                float scale[3],
                int color_tint = 0xffffffff
        ) = 0;

        virtual void Draw(
                Sprite* sprite,
                float position[3],
                float rotation[3],
                float scale[3],
                int color_tint = 0xffffffff
        ) = 0;

        virtual void EndDrawing() = 0;

        virtual ~IDraw() = default;

};

#endif // _OPENMAP_IDRAW_HPP