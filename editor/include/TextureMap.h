#ifndef TEXTURE_MAP_H
#define TEXTURE_MAP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2pp/SDL2pp.hh>
#include "Info.h"


#include <map>


class TextureMap {


private:

    std::map<int, SDL2pp::Texture> textures;


public:

    TextureMap(SDL2pp::Renderer &renderer);

    SDL2pp::Texture &getTexture(int texture_id);
};



#endif