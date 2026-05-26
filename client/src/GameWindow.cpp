#include "GameWindow.h"

#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <string>

std::unique_ptr<SDL2pp::Texture>
GameWindow::loadPlayerTexture(SDL2pp::Renderer &renderer,
                              const std::string &texturePath) {
  SDL2pp::Surface surface(texturePath);
  Uint32 colorKey = SDL_MapRGB(surface.Get()->format, 0, 0, 0);
  surface.SetColorKey(true, colorKey);

  auto texture = std::make_unique<SDL2pp::Texture>(renderer, surface);
  texture->SetBlendMode(SDL_BLENDMODE_BLEND);
  return texture;
}

GameWindow::GameWindow(uint32_t myPlayerID)
    : camera(Camera(720, 410)), myPlayerID(myPlayerID) {
  window = std::make_unique<SDL2pp::Window>(
      "Argentum Online", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 720,
      410, SDL_WINDOW_SHOWN);
  renderer =
      std::make_unique<SDL2pp::Renderer>(*window, -1, SDL_RENDERER_ACCELERATED);
  initResources();
}

void GameWindow::initResources() {
  backgroundTexture = std::make_unique<SDL2pp::Texture>(
      *renderer, SDL2pp::Surface("assets/10119.png"));
  defaultPlayerTexture = loadPlayerTexture(*renderer, "assets/11402.png");
}

void GameWindow::show(unsigned int it) {
  SDL_ClearError();
  renderer->Clear();
  renderer->Copy(*backgroundTexture, SDL2pp::Rect(0, 0, 400, 400),
                 SDL2pp::Rect(0, 0, 720, 410));
  render(it);
  renderer->Present();
}

void GameWindow::render(unsigned int it) {
  auto itMy = players.find(myPlayerID);
  if (itMy == players.end()) {
    throw std::runtime_error("My player not found in map");
  }

  const Player &myPlayer = *itMy->second;
  camera.follow(myPlayer.getX(), myPlayer.getY(), 32, 32);

  for (auto &entry : players) {
    if (entry.second) {
      renderPlayer(entry.first, entry.second, it);
    }
  }
}

void GameWindow::addPlayer(uint32_t ID, const Player *player) {
  players[ID] = player;
  auto texture = loadPlayerTexture(*renderer, headPathForRace(player->getRace()));
  headTextures[ID] = std::move(texture);
}

void GameWindow::renderPlayer(uint32_t playerId, const Player *player, unsigned int it) {
  const int animationIt = player->getIsMoving() ? static_cast<int>(it) : 0;
  SpriteFrame src =
      spriteFrameCalculator.getSprite(player->getDirection(), animationIt);
  SDL2pp::Rect r = camera.toScreen(player->getX(), player->getY(), 32, 32);
  renderer->Copy(*defaultPlayerTexture,
                 SDL2pp::Rect(src.x, src.y, src.w, src.h), r);

  auto headIt = headTextures.find(playerId);
  if (headIt == headTextures.end()) {
    return;
  }
  SpriteFrame headSrc = headFrameForDirection(player->getDirection());
  int headDestW = 24;
  int headDestH = 24;
  int headX =
      r.x + (r.w - headDestW) / 2 + headCenteringOffset(player->getDirection());
  int headY = r.y - headDestH + 4;
  SDL2pp::Rect headDest{headX, headY, headDestW, headDestH};
  renderer->Copy(*headIt->second,
                 SDL2pp::Rect(headSrc.x, headSrc.y, headSrc.w, headSrc.h),
                 headDest);
}

std::string GameWindow::headPathForRace(const std::string &race) const {
  std::string path = "assets/cabezas/";
  for (unsigned char c : race)
    path += std::tolower(c);
  path += ".png";
  return path;
}

SpriteFrame GameWindow::headFrameForDirection(Direction dir) {
  int x = 0;
  switch (dir) {
  case Direction::Down:
    x = 0;
    break;
  case Direction::Right:
    x = 17;
    break;
  case Direction::Left:
    x = 34;
    break;
  case Direction::Up:
    x = 51;
    break;
  }
  return {x, 0, 16, 16};
}

int GameWindow::headCenteringOffset(Direction dir) {
  switch (dir) {
  case Direction::Down:
    return 0;
  case Direction::Right:
    return 0;
  case Direction::Left:
    return -1;
  case Direction::Up:
    return 0;
  }
  return 0;
}
