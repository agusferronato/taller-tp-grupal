#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2pp/SDL2pp.hh>

#include "Camera.h"
#include "PlayerObserver.h"
#include "SpriteCalculator.h"
#include <string>

#include <memory>
#include <unordered_map>

class GameWindow {
private:
  SDL2pp::SDL sdl{SDL2pp::SDL(SDL_INIT_VIDEO)};
  SDL2pp::SDLTTF ttf;
  SDL2pp::SDLImage sdlimage{IMG_INIT_PNG};

  std::unique_ptr<SDL2pp::Window> window;
  std::unique_ptr<SDL2pp::Renderer> renderer;
  std::unique_ptr<SDL2pp::Texture> backgroundTexture;
  std::unique_ptr<SDL2pp::Texture> defaultPlayerTexture;

  Camera camera;
  SpriteFrameCalculator spriteFrameCalculator;
  uint32_t myPlayerID{0};
  std::unordered_map<uint32_t, const PlayerObserver *> players;

public:
  GameWindow();
  void setMyPlayerID(uint32_t id);
  void addPlayer(uint32_t ID, const PlayerObserver &player);
  void show(unsigned int it);

private:
  void clearDisplay();
  void render(unsigned int it);
  void initResources();
  std::unique_ptr<SDL2pp::Texture>
  loadPlayerTexture(SDL2pp::Renderer &renderer, const std::string &texturePath);
  void renderPlayer(const PlayerObserver &player, unsigned int it);
};

#endif
