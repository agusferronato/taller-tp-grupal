#ifndef TEXTURE_MAP_H
#define TEXTURE_MAP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2pp/SDL2pp.hh>
#include "Info.h"


#include <map>



struct TextureInMap {
    SDL2pp::Texture txt;
    int priority;
};


class TextureMap {


private:

    std::map<int, TextureInMap> textures;


public:

    TextureMap(SDL2pp::Renderer &renderer);

    TextureInMap& TextureMap::getTexture(int texture_id);
};



#endif