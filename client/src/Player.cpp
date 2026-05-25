#include "Player.h"

#include <cctype>

static std::string headPathForRace(const std::string &race) {
  std::string path = "assets/cabezas/";
  for (unsigned char c : race)
    path += std::tolower(c);
  path += ".png";
  return path;
}

Player::Player(SDL2pp::Renderer &renderer, uint32_t id,
               const std::string &texturePath, int xOrigin, int yOrigin,
               const std::string &race)
    : id(id), x(0), y(0),
      texture(renderer,
              [&]() {
                SDL2pp::Surface surface(texturePath);
                Uint32 colorKey = SDL_MapRGB(surface.Get()->format, 0, 0, 0);
                surface.SetColorKey(true, colorKey);
                return surface;
              }()),
      headTexture(renderer,
                  [&]() {
                    SDL2pp::Surface surface(headPathForRace(race));
                    Uint32 colorKey =
                        SDL_MapRGB(surface.Get()->format, 0, 0, 0);
                    surface.SetColorKey(true, colorKey);
                    return surface;
                  }()),
      race(race), it_init(0) {
  texture.SetBlendMode(SDL_BLENDMODE_BLEND);
  headTexture.SetBlendMode(SDL_BLENDMODE_BLEND);
  frame = spriteFrameCalculator.getSprite(Direction::Down, 0);
  x = xOrigin;
  y = yOrigin;
}

void Player::render(SDL2pp::Renderer &renderer, Camera &camera) {
  SDL2pp::Rect footRect = camera.toScreen(x, y, 32, 32);

  renderer.Copy(texture, SDL2pp::Rect(frame.x, frame.y, frame.w, frame.h),
                footRect);

  SpriteFrame headSrc = headFrameForDirection(direction);
  int headDestW = 24;
  int headDestH = 24;
  int headX =
      footRect.x + (footRect.w - headDestW) / 2 +
      headCenteringOffset(direction);
  int headY = footRect.y - headDestH + 4;
  SDL2pp::Rect headDest{headX, headY, headDestW, headDestH};
  renderer.Copy(headTexture,
                SDL2pp::Rect(headSrc.x, headSrc.y, headSrc.w, headSrc.h),
                headDest);
}
