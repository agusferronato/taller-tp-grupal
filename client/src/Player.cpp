#include "Player.h"

Player::Player(SDL2pp::Renderer& renderer, PlayerId id, const std::string& texturePath)
    : id(id), x(0), y(0),
      texture(renderer, [&](){
          SDL2pp::Surface surface(texturePath);
          Uint32 colorKey = SDL_MapRGB(surface.Get()->format, 0, 0, 0);
          surface.SetColorKey(true, colorKey);
          return surface;
      }()),
      it_init(0) {
    texture.SetBlendMode(SDL_BLENDMODE_BLEND);
    frame = spriteFrameCalculator.getSprite(Direction::Down, 0);
}