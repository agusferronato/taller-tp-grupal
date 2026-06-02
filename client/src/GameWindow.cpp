#include "GameWindow.h"

#include <cctype>
#include <fstream>
#include <stdexcept>
#include <string>

static std::string assetPath(const std::string &relative) {
  return "../client/" + relative;
}

std::unique_ptr<SDL2pp::Texture>
GameWindow::loadPlayerTexture(SDL2pp::Renderer& renderer,
                              const std::string& texturePath) {
  SDL2pp::Surface surface(texturePath);

  Uint32 colorKey = SDL_MapRGB(surface.Get()->format, 0, 0, 0);
  surface.SetColorKey(true, colorKey);

  auto texture = std::make_unique<SDL2pp::Texture>(renderer, surface);
  texture->SetBlendMode(SDL_BLENDMODE_BLEND);

  return texture;
}
GameWindow::GameWindow(uint32_t myPlayerID)
    : camera(Camera(680, 355)), myPlayerID(myPlayerID) {
  window = std::make_unique<SDL2pp::Window>(
      "Argentum Online",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      960,
      540,
      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  renderer =
      std::make_unique<SDL2pp::Renderer>(*window, -1, SDL_RENDERER_ACCELERATED);
  
  // Mantiene la relacion de aspecto aunque se redimensione la ventana
  renderer->SetLogicalSize(960, 540);

  initResources();
}

void GameWindow::initResources() {
  font = std::make_unique<SDL2pp::Font>(
      assetPath("fonts/Vera.ttf"), 14);

  titleFont = std::make_unique<SDL2pp::Font>(
      assetPath("fonts/OldLondon.ttf"), 34);

  uiFont = std::make_unique<SDL2pp::Font>(
      assetPath("fonts/CinzelBold.ttf"), 18);

  textureMapper = std::make_unique<TextureMapper>(*renderer);
  textureMapper->loadFromToml("assets/textures.toml");

  chatView = std::make_unique<GameChatView>(font.get());

  uiFrameTexture = std::make_unique<SDL2pp::Texture>(
      *renderer,
      SDL2pp::Surface(assetPath("assets/HUD/UpperLayer.png")));

  chatMessagesBackground = std::make_unique<SDL2pp::Texture>(
    *renderer,
    SDL2pp::Surface(assetPath("assets/HUD/UserChat/base_messages.png")));

  chatInputBackground = std::make_unique<SDL2pp::Texture>(
      *renderer,
      SDL2pp::Surface(assetPath("assets/HUD/UserChat/base_input.png")));

  userInfoBackground = std::make_unique<SDL2pp::Texture>(
      *renderer,
      SDL2pp::Surface(assetPath("assets/HUD/UserInfo/base.png")));

  userInventoryBackground = std::make_unique<SDL2pp::Texture>(
      *renderer,
      SDL2pp::Surface(assetPath("assets/HUD/UserInventory/base.png")));

  userStatsBackground = std::make_unique<SDL2pp::Texture>(
      *renderer,
      SDL2pp::Surface(assetPath("assets/HUD/UserStats/base.png")));
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

  Layout layout = getLayout();

  render(it);
  renderUIFrame(layout);

  renderer->Present();
}
void GameWindow::renderHUD() {
  Layout layout = getLayout();

  renderChat(layout);
  renderPlayerHeader(layout);
  renderPlayerStats(layout);
  renderInventoryInfo(layout);
  renderVitals(layout);
}

void GameWindow::renderCommonGround() {
  for (int i = 0; i < maxSize; i++) {
    for (int j = 0; j < maxSize; j++) {
      SDL2pp::Rect dstRect = camera.toScreen(
          (i - maxSize / 2) * gridSize,
          (j - maxSize / 2) * gridSize,
          gridSize,
          gridSize);

      SDL2pp::Rect srcRect = {0, 0, gridSize, gridSize};

      renderer->Copy(textureMapper->getTexture(commonGroundTextureId),
                     srcRect,
                     dstRect);
    }
  }
}

void GameWindow::render(unsigned int it) {
  auto itMy = players.find(myPlayerID);
  if (itMy == players.end()) {
    throw std::runtime_error("My player not found in map");
  }

  Layout layout = getLayout();

  Player &myPlayer = *itMy->second;

  camera.setViewport(
      layout.gameRect.GetX(),
      layout.gameRect.GetY(),
      layout.gameRect.GetW(),
      layout.gameRect.GetH());

  camera.follow(myPlayer.get_x(), myPlayer.get_y(), 32, 32);

  renderWorld(it);
  renderUIBackgrounds(layout);
  renderHUD();
}

void GameWindow::clear() {
  for (auto& entity : entities) {
    entity->clear();
  }

  renderer->SetDrawColor(0, 0, 0, 255);
  renderer->Clear();
}

void GameWindow::addPlayer(uint32_t id, Player* player) {
  players[id] = player;

  auto bodyTexture = loadPlayerTexture(
      *renderer,
      assetPath("assets/11402.png"));

  player->setPlayerTexture(std::move(bodyTexture));

  auto headTexture = loadPlayerTexture(
      *renderer,
      assetPath(headPathForRace(player->getRace())));

  player->setHeadTexture(std::move(headTexture));
  player->setNameFont(font.get());

  entities.push_back(player);
}

void GameWindow::removePlayer(uint32_t id) {
  auto it = players.find(id);
  if (it != players.end()) {
    entities.remove(it->second);
    players.erase(it);
  }
}

std::string GameWindow::headPathForRace(const std::string& race) const {
  std::string path = "assets/cabezas/";
  for (unsigned char c : race) {
    path += std::tolower(c);
  }
  path += ".png";
  return path;
}

void GameWindow::setChatState(const std::deque<std::string>& messages,
                              const std::string& input,
                              bool active) {
  chatMessages = messages;
  currentChatInput = input;
  chatActive = active;
}



// Layout and rendering helpers
GameWindow::Layout GameWindow::getLayout() const {
  return Layout{
      SDL2pp::Rect(0, 0, 960, 540),

      // Chat messages
      SDL2pp::Rect(3, 3, 676, 113),
      
      // Chat input
      SDL2pp::Rect(3, 122, 676, 20), 
      
      // Juego principal
      SDL2pp::Rect(8, 145, 668, 385),

      // Informacion/Experiencia
      SDL2pp::Rect(686, 7, 267, 114),

      // Inventario/Equipamiento/Oro
      SDL2pp::Rect(686, 128, 267, 294),

      // Vida/Mana
      SDL2pp::Rect(686, 429, 267, 104)
  };
}

void GameWindow::renderUIFrame(const Layout& layout) {
  if (!uiFrameTexture)
    return;

  renderer->Copy(*uiFrameTexture, SDL2pp::NullOpt, layout.windowRect);
}

void GameWindow::renderWorld(unsigned int it) {
  renderCommonGround();
  renderEntitiesByPriority(it);
}

void GameWindow::renderEntitiesByPriority(unsigned int it) {
  for (size_t i = 0; i < tilesToRender.size(); i++) {
    auto& priority = tilesToRender[i];

    for (auto &[pair, items] : priority) {
      int max_row = pair.first;
      int y_max = (max_row - maxSize / 2 + 1) * gridSize;

      for (auto& entity : entities) {
        if (!entity->rendered() &&
            entity->get_y() + 1.25 * entity->get_h() < y_max &&
            entity->hasPriority(i)) {
          entity->render(*renderer, camera, it);
        }
      }

      for (auto &item : items) {
        SDL2pp::Rect dstRect = camera.toScreen(
            (item.i - maxSize / 2) * gridSize,
            (item.j - maxSize / 2) * gridSize,
            gridSize,
            gridSize);

        SDL2pp::Rect srcRect = {
            item.x_start,
            item.y_start,
            item.x_end - item.x_start,
            item.y_end - item.y_start
        };

        renderer->Copy(textureMapper->getTexture(item.texture_id),
                       srcRect,
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
}

void GameWindow::renderChat(const Layout& layout) {
  if (!chatView)
    return;

  chatView->render(
      *renderer,
      layout.chatMessagesRect,
      layout.chatInputRect,
      chatMessages,
      currentChatInput,
      chatActive);
}

void GameWindow::renderText(int x,
                            int y,
                            const std::string& text,
                            SDL_Color color) {
  if (!font)
    return;

  SDL2pp::Surface surf =
      font->RenderUTF8_Solid(text, color);

  SDL2pp::Texture tex(*renderer, surf);

  renderer->Copy(
      tex,
      SDL2pp::NullOpt,
      SDL2pp::Rect(
          x,
          y,
          surf.GetWidth(),
          surf.GetHeight()));
}

void GameWindow::drawBar(int x,
                         int y,
                         int w,
                         int h,
                         uint32_t cur,
                         uint32_t max,
                         SDL_Color fg,
                         SDL_Color bg) {
  SDL2pp::Rect bgRect(x, y, w, h);

  renderer->SetDrawColor(
      bg.r, bg.g, bg.b, bg.a);

  renderer->FillRect(bgRect);

  if (max == 0)
    return;

  int fillW =
      static_cast<int>(
          (static_cast<double>(cur) / max) * w);

  if (fillW <= 0)
    return;

  SDL2pp::Rect fillRect(x, y, fillW, h);

  renderer->SetDrawColor(
      fg.r, fg.g, fg.b, fg.a);

  renderer->FillRect(fillRect);
}

void GameWindow::renderPlayerStats(const Layout& layout) {
  auto itMy = players.find(myPlayerID);
  if (itMy == players.end())
    return;

  const Player& p = *itMy->second;

  int xpCur = 357;   // placeholder, reemplazar por xp actual
  int xpMax = 1000;  // placeholder, reemplazar por xp total para subir de nivel
  // Para el nivel maximo se podria hacer que la barra aparezca siempre llena por ejemplo

  int x = layout.rightTopRect.GetX();
  int y = layout.rightTopRect.GetY();

  renderText(
      x + 40,
      y + 28,
      std::to_string(p.getLevel()),
      SDL_Color{255, 255, 200, 255});

  int xpX = x + 20;
  int xpY = y + 84;
  int xpW = 227;
  int xpH = 20;
  // int xpX = x + 20 + 2;
  // int xpY = y + 84 + 2;
  // int xpW = 227 - 2;
  // int xpH = 20 - 2;

  
  drawBar(xpX, xpY, xpW, xpH,
    xpCur, xpMax,
    SDL_Color{60, 180, 60, 255},
    SDL_Color{20, 20, 20, 255});
    
  SDL2pp::Rect xpBarRect(xpX, xpY, xpW, xpH);
  renderCenteredTextInRect(
      xpBarRect,
      std::to_string(xpCur) + " / " + std::to_string(xpMax),
      SDL_Color{255, 255, 255, 255});
}

void GameWindow::renderVitals(const Layout& layout) {
  auto itMy = players.find(myPlayerID);
  if (itMy == players.end())
    return;

  const Player& p = *itMy->second;

  int x = layout.bottomRightRect.GetX();
  int y = layout.bottomRightRect.GetY();

  int barX = x + 20;
  int barW = 227;
  int barH = 20;

  int hpY = y + 30;
  drawBar(barX, hpY, barW, barH,
          p.getHp()-15, p.getMaxHp(),
          SDL_Color{200,40,40,255},
          SDL_Color{30,0,0,255});

  SDL2pp::Rect hpBarRect(barX, hpY, barW, barH);
  renderCenteredTextInRect(
      hpBarRect,
      std::to_string(p.getHp()-15) + "/" + std::to_string(p.getMaxHp()),
      SDL_Color{255,255,255,255});

  int manaY = y + 74;
  drawBar(barX, manaY, barW, barH,
          p.getMana(), p.getMaxMana(),
          SDL_Color{40,80,220,255},
          SDL_Color{0,0,40,255});

  SDL2pp::Rect manaBarRect(barX, manaY, barW, barH);
  renderCenteredTextInRect(
      manaBarRect,
      std::to_string(p.getMana()) + "/" + std::to_string(p.getMaxMana()),
      SDL_Color{255,255,255,255});
}

void GameWindow::renderPlayerHeader(const Layout& layout) {
  if (!titleFont)
    return;

  SDL2pp::Surface surf =
      titleFont->RenderUTF8_Blended(
          "Argentum",
          SDL_Color{255, 255, 255, 255});

  SDL2pp::Texture tex(*renderer, surf);

  int x = layout.rightTopRect.GetX() + 24 +
          (layout.rightTopRect.GetW() - surf.GetWidth()) / 2;

  int y = layout.rightTopRect.GetY() + 18;

  renderer->Copy(
      tex,
      SDL2pp::NullOpt,
      SDL2pp::Rect(x, y, surf.GetWidth(), surf.GetHeight()));
}

void GameWindow::renderUIBackgrounds(const Layout& layout) {
  if (chatMessagesBackground) {
    renderer->Copy(*chatMessagesBackground,
                   SDL2pp::NullOpt,
                   layout.chatMessagesRect);
  }

  if (chatInputBackground) {
    renderer->Copy(*chatInputBackground,
                   SDL2pp::NullOpt,
                   layout.chatInputRect);
  }

  if (userInfoBackground) {
    renderer->Copy(*userInfoBackground,
                   SDL2pp::NullOpt,
                   layout.rightTopRect);
  }

  if (userInventoryBackground) {
    renderer->Copy(*userInventoryBackground,
                   SDL2pp::NullOpt,
                   layout.inventoryRect);
  }

  if (userStatsBackground) {
    renderer->Copy(*userStatsBackground,
                   SDL2pp::NullOpt,
                   layout.bottomRightRect);
  }
}

void GameWindow::renderInventoryInfo(const Layout& layout) {
  auto itMy = players.find(myPlayerID);
  if (itMy == players.end())
    return;

  // Cuando deje de estar mockeado el oro, reemplazar por p.getGold() o similar
  // const Player& p = *itMy->second;

  int insuredGold = 399; // placeholder
  int excessGold = 0;    // placeholder

  int x = layout.inventoryRect.GetX();
  int y = layout.inventoryRect.GetY();

  SDL2pp::Rect insuredRect(x + 70, y + 260, 60, 22);
  SDL2pp::Rect excessRect(x + 190, y + 260, 60, 22);

  renderCenteredTextInRect(
      insuredRect,
      std::to_string(insuredGold),
      SDL_Color{255, 255, 200, 255});

  renderCenteredTextInRect(
      excessRect,
      std::to_string(excessGold),
      SDL_Color{255, 255, 200, 255});
}

void GameWindow::renderCenteredTextInRect(const SDL2pp::Rect& rect,
                                          const std::string& text,
                                          SDL_Color color) {
  if (!font)
    return;

  SDL2pp::Surface surf = font->RenderUTF8_Solid(text, color);
  SDL2pp::Texture tex(*renderer, surf);

  int x = rect.GetX() + (rect.GetW() - surf.GetWidth()) / 2;
  int y = rect.GetY() + (rect.GetH() - surf.GetHeight()) / 2;

  renderer->Copy(
      tex,
      SDL2pp::NullOpt,
      SDL2pp::Rect(x, y, surf.GetWidth(), surf.GetHeight()));
}

// WIP
void GameWindow::renderInventoryPanel([[maybe_unused]] const Layout& layout) {}