#include "GameWindow.h"

GameWindow::GameWindow() : camera(Camera(720, 410)) {
  this->window = std::make_unique<SDL2pp::Window>(
      SDL2pp::Window("Argentum Online", SDL_WINDOWPOS_UNDEFINED,
                     SDL_WINDOWPOS_UNDEFINED, 720, 410, SDL_WINDOW_SHOWN));

  this->renderer = std::make_unique<SDL2pp::Renderer>(
      SDL2pp::Renderer(*window, -1, SDL_RENDERER_ACCELERATED));

  backgroundTexture = std::make_unique<SDL2pp::Texture>(
      *renderer, SDL2pp::Surface("assets/10119.png"));
  initResources();
}

void GameWindow::initResources() {
  // precarga textura por defecto para jugadores
  defaultPlayerTexture = std::make_unique<SDL2pp::Texture>(
      *renderer, SDL2pp::Surface("assets/11402.png"));
}

void GameWindow::show(unsigned int it) {
  clearDisplay();
  render(it);
}

void GameWindow::render(unsigned int it) {
  auto itMy = players.find(myPlayerID);
  if (itMy == players.end() || !itMy->second) {
    throw std::runtime_error("My player not found in view");
  }
  const PlayerObserver &myPlayer = *itMy->second;
  camera.follow(myPlayer.getX(), myPlayer.getY(), 32, 32);

  for (auto &entry : players) {
    if (entry.second) {
      renderPlayer(*entry.second);
    }
  }

  renderer->Present();
}

void GameWindow::clearDisplay() {
  if (renderer) {
    renderer->Copy(*backgroundTexture, SDL2pp::Rect(0, 0, 400, 400),
                   SDL2pp::Rect(0, 0, 720, 410));
  }
}

void GameWindow::setMyPlayerID(uint32_t id) { this->myPlayerID = id; }

void GameWindow::addPlayer(uint32_t ID, const PlayerObserver &player) {
  players[ID] = std::make_unique<PlayerObserver>(player);
}

void GameWindow::renderPlayer(const PlayerObserver &player) {
  SDL2pp::Rect r = camera.toScreen(player.getX(), player.getY(), 32, 32);
  SDL2pp::Rect src(0, 0, 32, 32);
  if (defaultPlayerTexture && renderer) {
    renderer->Copy(*defaultPlayerTexture, src, r);
  }
}
