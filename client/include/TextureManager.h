#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANANGER_H


class TextureManager {

private:
    std::map<int, SDL2pp::Texture> textures;
    std::map<TextureLayoutType, TextureLayout> texturesFrames;

public:

    TextureManager() {

        /*
        
        1)
        Open TOML file /assets/sprites.toml
        
        TOML file format:

        texture:

        texture_id,
        layout_type // Body, Head, ...
        array: [ // this is an example
            direction up: [
                { x, y, w, h }, // frame1
                { x, y, w, h },// frame2
                { x, y, w, h }, // frame3
                { x, y, w, h }, // frame4
            ]
        ]


        2) Parsing

        a- Include texture in textures map (with transparency)
        b- switch (TextureLayoutType) {
        
            case TextureLayoutType::Body:
                ....
                include vector in layouts map

            case TextureLayoutType::Head:
                ....
        
        
        }



        
        
        */



    }

    Sprite getBodySprite();
    Sprite getHeadSprite();
    Sprite getSprite();

};




#endif
