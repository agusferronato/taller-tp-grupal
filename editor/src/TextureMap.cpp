#include "TextureMap.h"


TextureMap::TextureMap(SDL2pp::Renderer& renderer) {

    this->textures.insert({
        GRASS_TEXTURE_ID,
        SDL2pp::Texture(renderer, SDL2pp::Surface("assets/background/ground/114.png"))
    });
    
    SDL2pp::Surface surface("assets/background/decoration/10.png");
    surface.SetColorKey(true, SDL_MapRGB(surface.Get()->format, 0, 0, 0));
    this->textures.insert({
        1,
        SDL2pp::Texture(renderer, surface)
    });
        
}



SDL2pp::Texture& TextureMap::getTexture(int texture_id)
{
    return textures.at(texture_id);
}
