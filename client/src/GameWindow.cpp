#include "GameWindow.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <stdexcept>
#include <string>

#include "Player.h"
#include "PlayerEntity.h"

GameWindow::GameWindow(uint32_t myPlayerID, int windowWidth, int windowHeight)
    : camera(Camera(windowWidth, windowHeight)), myPlayerID(myPlayerID) {
  window = std::make_unique<SDL2pp::Window>(
      "Argentum Online", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      windowWidth, windowHeight, SDL_WINDOW_SHOWN);
  renderer =
      std::make_unique<SDL2pp::Renderer>(*window, -1, SDL_RENDERER_ACCELERATED);

  this->windowWidth = windowWidth;
  this->windowHeight = windowHeight;

  initResources();
}

void GameWindow::initResources() {
  font = std::make_unique<SDL2pp::Font>("fonts/Vera.ttf", 12);

  textureMapper = std::make_unique<TextureMapper>(*renderer);
  textureMapper->loadFromToml("assets/textures.toml");
}

void GameWindow::addEntity(EntityType type, uint32_t id,
                           std::unique_ptr<RenderableEntity> entity) {
  EntityKey key(type, id);
  entities[key] = std::move(entity);
}

void GameWindow::removeEntity(EntityType type, uint32_t id) {
  EntityKey key(type, id);
  if (myPlayerEntity && type == EntityType::Player && id == myPlayerID) {
    myPlayerEntity = nullptr;
  }
  entities.erase(key);
}

void GameWindow::setMyPlayer(PlayerEntity *entity) { myPlayerEntity = entity; }

SDL2pp::Renderer &GameWindow::getRenderer() { return *renderer; }

SDL2pp::Font &GameWindow::getFont() { return *font; }

void GameWindow::setMapData(int maxSize, int gridSize,
                            int commonGroundTextureId,
                            const std::list<TileOrigin> &origins) {
  this->maxSize = maxSize;
  this->gridSize = gridSize;
  this->commonGroundTextureId = commonGroundTextureId;
  textureMapper->buildRenderGrid(origins, gridSize);
  tilesToRender = textureMapper->getTilesToRender();
}

void GameWindow::show(unsigned int it) {
  SDL_ClearError();
  clear();

  renderer->SetDrawColor(0, 0, 0, 255);
  renderer->FillRect(SDL2pp::Rect(0, 0, 720, 410));

  render(it);
  renderer->Present();
}

void GameWindow::renderHUD() {
  if (!myPlayerEntity)
    return;
  const Player &p = myPlayerEntity->getPlayer();

  int barW = 180;
  int barH = 16;
  int barX = 10;
  int barY = 10;

  auto drawBar = [&](int x, int y, int w, int h, uint32_t cur, uint32_t max,
                     SDL_Color fg, SDL_Color bg) {
    SDL2pp::Rect bgRect(x, y, w, h);
    renderer->SetDrawColor(bg.r, bg.g, bg.b, bg.a);
    renderer->FillRect(bgRect);
    if (max > 0) {
      int fillW = static_cast<int>((static_cast<double>(cur) / max) * w);
      if (fillW > 0) {
        SDL2pp::Rect fillRect(x, y, fillW, h);
        renderer->SetDrawColor(fg.r, fg.g, fg.b, fg.a);
        renderer->FillRect(fillRect);
      }
    }
  };

  drawBar(barX, barY, barW, barH, p.getHp(), p.getMaxHp(),
          SDL_Color{200, 40, 40, 255}, SDL_Color{60, 10, 10, 255});
  drawBar(barX, barY + barH + 2, barW, barH, p.getMana(), p.getMaxMana(),
          SDL_Color{40, 80, 220, 255}, SDL_Color{10, 20, 60, 255});
  drawBar(barX, barY + 2 * (barH + 2), barW, barH, p.getExperience(),
          1000 * static_cast<uint32_t>(
                     std::pow(static_cast<double>(p.getLevel()), 1.5)),
          SDL_Color{60, 200, 60, 255}, SDL_Color{10, 50, 10, 255});

  auto renderText = [&](int x, int y, const std::string &text,
                        SDL_Color color) {
    SDL2pp::Surface surf = font->RenderUTF8_Solid(text, color);
    SDL2pp::Texture tex(*renderer, surf);
    renderer->Copy(tex, SDL2pp::NullOpt,
                   SDL2pp::Rect(x, y, surf.GetWidth(), surf.GetHeight()));
  };

  renderText(barX + barW + 6, barY, "Lv " + std::to_string(p.getLevel()),
             SDL_Color{255, 255, 200, 255});
  renderText(barX + barW + 6, barY + barH + 2,
             std::to_string(p.getHp()) + "/" + std::to_string(p.getMaxHp()),
             SDL_Color{255, 200, 200, 255});
  renderText(barX + barW + 6, barY + 2 * (barH + 2),
             std::to_string(p.getMana()) + "/" + std::to_string(p.getMaxMana()),
             SDL_Color{200, 200, 255, 255});

  renderText(570, 10, "Oro: " + std::to_string(p.getGold()),
             SDL_Color{255, 215, 0, 255});
}

void GameWindow::renderCommonGround() {
  for (int i = 0; i < maxSize; i++) {
    for (int j = 0; j < maxSize; j++) {
      SDL2pp::Rect dstRect =
          camera.toScreen((i - maxSize / 2) * gridSize,
                          (j - maxSize / 2) * gridSize, gridSize, gridSize);

      SDL2pp::Rect srcRect = {0, 0, gridSize, gridSize};
      renderer->Copy(textureMapper->getTexture(commonGroundTextureId), srcRect,
                     dstRect);
    }
  }
}

void GameWindow::getSortedEntities(
    std::vector<RenderableEntity *> &sortedEntities) {
  sortedEntities.reserve(entities.size());

  for (auto &[key, entity] : entities) {
    sortedEntities.push_back(entity.get());
  }
  std::sort(sortedEntities.begin(), sortedEntities.end(),
            [](RenderableEntity *a, RenderableEntity *b) {
              if (a->get_y() != b->get_y())
                return a->get_y() < b->get_y();
              return a->get_x() < b->get_x();
            });
}

void GameWindow::render(unsigned int it) {
  if (myPlayerEntity) {
    camera.follow(myPlayerEntity->get_x(), myPlayerEntity->get_y(),
                  Player::Width, Player::Height);
  }

  renderCommonGround();

  std::vector<RenderableEntity *> sortedEntities;
  getSortedEntities(sortedEntities);

  for (size_t i = 0; i < tilesToRender.size(); i++) {
    auto &priority = tilesToRender[i];

    for (auto &[pair, items] : priority) {
      int max_row = pair.first;
      int y_max = (max_row - maxSize / 2 + 1) * gridSize;

      for (auto *entity : sortedEntities) {
        if (!entity->rendered() &&
            entity->get_y() + 1.25 * entity->get_h() < y_max &&
            entity->hasPriority(i)) {
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
    for (auto *entity : sortedEntities) {
      if (!entity->rendered() && entity->hasPriority(i)) {
        entity->render(*renderer, camera, it);
      }
    }
  }

  renderHUD();
}

void GameWindow::clear() {
  for (auto &[key, entity] : entities) {
    entity->clear();
  }
  renderer->Clear();
}
