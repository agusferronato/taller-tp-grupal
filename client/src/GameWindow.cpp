#include "GameWindow.h"
#include <stdexcept>

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
  Player::setPlayerTexture(defaultPlayerTexture.get());

  textureMapper = std::make_unique<TextureMapper>(*renderer);
  textureMapper->loadFromToml("assets/textures.toml");
}

void GameWindow::setMapData(int maxSize_, int gridSize_,
                            int commonGroundTextureId_,
                            const std::list<TileOrigin> &origins) {
  maxSize = maxSize_;
  gridSize = gridSize_;
  commonGroundTextureId = commonGroundTextureId_;
  textureMapper->buildRenderGrid(origins, gridSize);
  tilesToRender = textureMapper->getTilesToRender();
}

void GameWindow::show(unsigned int it) {
  SDL_ClearError();
  clear();
  renderer->Copy(*backgroundTexture, SDL2pp::Rect(0, 0, 400, 400),
                 SDL2pp::Rect(0, 0, 720, 410));
  render(it);
  renderer->Present();
}

void GameWindow::renderCommonGround() {
  for (int i = 0; i < maxSize; i++) {
    for (int j = 0; j < maxSize; j++) {
      SDL2pp::Rect dstRect = camera.toScreen(
          (i - maxSize / 2) * gridSize, (j - maxSize / 2) * gridSize,
          gridSize, gridSize);

      SDL2pp::Rect srcRect = {0, 0, gridSize, gridSize};
      renderer->Copy(textureMapper->getTexture(commonGroundTextureId),
                     srcRect, dstRect);
    }
  }
}

void GameWindow::render(unsigned int it) {
  auto itMy = players.find(myPlayerID);
  if (itMy == players.end()) {
    throw std::runtime_error("My player not found in map");
  }

  Player &myPlayer = *itMy->second;
  camera.follow(myPlayer.get_x(), myPlayer.get_y(), 32, 32);

  renderCommonGround();

  for (size_t i = 0; i < tilesToRender.size(); i++) {
    auto& priority = tilesToRender[i];

    for (auto &[pair, items] : priority) {

      int max_row = pair.first;
      int y_max = (max_row - maxSize / 2 + 1) * gridSize;

      for (auto& entity : entities) {
          if (!entity->rendered() && entity->get_y() + 1.25 * entity->get_h() < y_max && entity->hasPriority(i)) {
              entity->render(*renderer, camera, it);
          }
      }

      for (auto &item : items) {
        SDL2pp::Rect dstRect = camera.toScreen(
            (item.i - maxSize / 2) * gridSize,
            (item.j - maxSize / 2) * gridSize, gridSize, gridSize);

        SDL2pp::Rect srcRect = {item.x_start, item.y_start,
                                item.x_end - item.x_start,
                                item.y_end - item.y_start};
        renderer->Copy(textureMapper->getTexture(item.texture_id), srcRect,
                       dstRect);
      }
    }
    for (auto& entity : entities) {
      if (!entity->rendered() && entity->hasPriority(i)) {
        entity->render(*renderer, camera, it);
      }
    }
  }
  
}

void GameWindow::clear()
{
  for (auto& entity : entities) {
    entity->clear();
  }
  renderer->Clear();
}

void GameWindow::addPlayer(uint32_t ID, Player *player) {
  players[ID] = player;
  entities.push_back(player);
}

