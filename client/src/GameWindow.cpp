#include "GameWindow.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <stdexcept>
#include <string>

static std::string assetPath(const std::string &relative) {
  return "../client/" + relative;
}

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
      *renderer, SDL2pp::Surface(assetPath("assets/10119.png")));
  font = nullptr;
  std::ifstream sys("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
  if (sys.good()) {
    font = std::make_unique<SDL2pp::Font>("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 14);
  } else {
    std::ifstream veraf(assetPath("fonts/Vera.ttf").c_str());
    if (veraf.good())
      font = std::make_unique<SDL2pp::Font>(assetPath("fonts/Vera.ttf"), 14);
  }

  textureMapper = std::make_unique<TextureMapper>(*renderer);
  textureMapper->loadFromToml("assets/textures.toml");
  chatView = std::make_unique<GameChatView>(font.get());
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

void GameWindow::renderHUD() {
  auto itMy = players.find(myPlayerID);
  if (itMy == players.end())
    return;
  const Player &p = *itMy->second;

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

  chatView->render(
    *renderer,
    SDL2pp::Rect(0, 280, 720, 130),
    std::deque<std::string>{"Hola", "Mensaje de prueba"},
    "",
    false
  );
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

  for (auto& entity : entities) {
    if (!entity->rendered()) {
      entity->render(*renderer, camera, it);
    }
  }

  renderHUD();
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
  auto bodyTexture = loadPlayerTexture(*renderer,
                                        assetPath("assets/11402.png"));
  player->setPlayerTexture(std::move(bodyTexture));

  auto headTexture = loadPlayerTexture(*renderer,
                                       assetPath(headPathForRace(player->getRace())));
  player->setHeadTexture(std::move(headTexture));
  player->setNameFont(font.get());
  entities.push_back(player);
}

void GameWindow::removePlayer(uint32_t ID) {
  auto it = players.find(ID);
  if (it != players.end()) {
    entities.remove(it->second);
  }
  players.erase(ID);
}

std::string GameWindow::headPathForRace(const std::string &race) const {
  std::string path = "assets/cabezas/";
  for (unsigned char c : race)
    path += std::tolower(c);
  path += ".png";
  return path;
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
