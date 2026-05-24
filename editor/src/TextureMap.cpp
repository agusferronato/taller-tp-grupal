#include "TextureMap.h"


TextureMap::TextureMap(SDL2pp::Renderer& renderer) {

    this->textures.insert({
        GRASS_TEXTURE_ID,
        SDL2pp::Texture(renderer, SDL2pp::Surface("assets/10119.png"))
    });
        
}



SDL2pp::Texture& TextureMap::getTexture(int texture_id)
{
    return textures.at(texture_id);
}
