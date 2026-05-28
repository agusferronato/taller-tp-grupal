#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2pp/SDL2pp.hh>
#include <list>

#include "Camera.h"
#include "Player.h"
#include "SpriteCalculator.h"

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

class GameWindow {
private:
  SDL2pp::SDL sdl{SDL_INIT_VIDEO};
  SDL2pp::SDLImage sdlimage{IMG_INIT_PNG};
  SDL2pp::SDLTTF ttf;

  std::unique_ptr<SDL2pp::Window> window;
  std::unique_ptr<SDL2pp::Renderer> renderer;
  std::unique_ptr<SDL2pp::Texture> backgroundTexture;
  std::unique_ptr<SDL2pp::Texture> defaultPlayerTexture;

  Camera camera;
  SpriteFrameCalculator spriteFrameCalculator;
  uint32_t myPlayerID{0};
  std::unordered_map<uint32_t, Player *> players;
  std::list<RenderableEntity> entities;

public:
  explicit GameWindow(uint32_t myPlayerID);
  void addPlayer(uint32_t ID, Player *player);
  void show(unsigned int it);

private:
  void render(unsigned int it);
  void initResources();
  std::unique_ptr<SDL2pp::Texture>
  loadPlayerTexture(SDL2pp::Renderer &renderer, const std::string &texturePath);
};

#endif
