#ifndef _OPENMAP_IDRAW_HPP
#define _OPENMAP_IDRAW_HPP


class IDraw {

public:
        typedef struct Model;
        typedef struct Sprite;


        virtual Model* LoadModel(const char* target_file_path);
        virtual void UnloadModel(Model* model);

        virtual Sprite* LoadSprite(const char* target_file_path);
        virtual void UnloadSprite(Sprite* sprite);


        virtual void ToggleCameraProjection(); // TODO


        virtual bool WindowShouldClose();

        virtual void BeginDrawing();

        virtual void Draw(
                Model* model,
                float position[3],
                float rotation[3],
                float scale[3],
                int color_tint = 0xffffffff
        );

        virtual void Draw(
                Sprite* sprite,
                float position[3],
                float rotation[3],
                float scale[3],
                int color_tint = 0xffffffff
        );

        virtual void EndDrawing();

};

#endif // _OPENMAP_IDRAW_HPP