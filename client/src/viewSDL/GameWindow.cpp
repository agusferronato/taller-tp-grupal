#include "GameWindow.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <string>

#include "CityEntityModel.h"
#include "CityEntityRenderable.h"
#include "NPCEntity.h"
#include "PlayerEntity.h"

namespace {
constexpr int kMargin = 8;
constexpr int kChatGap = 4;
constexpr int kSidePanelWidth = 267;
constexpr int kChatMessagesHeight = 112;
constexpr int kChatInputHeight = 32;
constexpr int kInventoryPanelHeight = 294;
constexpr int kInfoPanelHeight = 114;
constexpr int kStatsPanelHeight = 104;

constexpr SDL_Color kTitleColor{255, 255, 255, 255};
constexpr SDL_Color kHeaderAccent{255, 223, 128, 255};
constexpr SDL_Color kPrimaryText{238, 232, 213, 255};
constexpr SDL_Color kHighlightText{255, 215, 0, 255};
constexpr SDL_Color kHpFill{200, 40, 40, 255};
constexpr SDL_Color kHpBg{60, 10, 10, 255};
constexpr SDL_Color kManaFill{40, 80, 220, 255};
constexpr SDL_Color kManaBg{10, 20, 60, 255};
constexpr SDL_Color kExpFill{60, 200, 60, 255};
constexpr SDL_Color kExpBg{10, 50, 10, 255};
} // namespace

GameWindow::GameWindow(uint32_t myPlayerID)
    : camera(Camera(960, 540)),
      myPlayerID(myPlayerID),
      windowWidth(960),
      windowHeight(540) {
  window = std::make_unique<SDL2pp::Window>(
      "Argentum Online", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960,
      540, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  renderer =
      std::make_unique<SDL2pp::Renderer>(*window, -1, SDL_RENDERER_ACCELERATED);

  renderer->SetLogicalSize(960, 540);
  renderer->SetDrawBlendMode(SDL_BLENDMODE_BLEND);

  initResources();
}

void GameWindow::initResources() {
  font = std::make_unique<SDL2pp::Font>("fonts/Vera.ttf", 14);
  titleFont = std::make_unique<SDL2pp::Font>("fonts/OldLondon.ttf", 34);
  uiFont = std::make_unique<SDL2pp::Font>("fonts/CinzelBold.ttf", 18);

  chatView = std::make_unique<GameChatView>(font.get());

  auto loadTexture = [this](const std::string &path) {
    return std::make_unique<SDL2pp::Texture>(*renderer, SDL2pp::Surface(path));
  };

  uiFrameTexture = loadTexture("assets/HUD/UpperLayer.png");
  chatMessagesBackground = loadTexture("assets/HUD/UserChat/base_messages.png");
  chatInputBackground = loadTexture("assets/HUD/UserChat/base_input.png");
  userInfoBackground = loadTexture("assets/HUD/UserInfo/base.png");
  userInventoryBackground = loadTexture("assets/HUD/UserInventory/base.png");
  userStatsBackground = loadTexture("assets/HUD/UserStats/base.png");

  textureMapper = std::make_unique<TextureMapper>(*renderer);
  textureMapper->loadFromToml("assets/textures.toml");

  textureManager = std::make_unique<TextureManager>(*renderer);
  textureManager->loadLayoutsFromToml("assets/layouts.toml");
  textureManager->loadTexturesFromToml("assets/sprites.toml");

  invPanel = std::make_unique<InventoryPanel>(*renderer, *textureManager);
  invPanel->loadTextures();
}

SDL2pp::Renderer &GameWindow::getRenderer() {
  return *renderer;
}

SDL2pp::Font &GameWindow::getFont() {
  return *font;
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

void GameWindow::setChatState(const std::deque<std::string> &messages,
                              const std::string &input, bool active) {
  chatMessages = messages;
  currentChatInput = input;
  chatActive = active;
}

void GameWindow::scrollChatUp() {
  if (chatView)
    chatView->scrollChatUp();
}

void GameWindow::scrollChatDown() {
  if (chatView)
    chatView->scrollChatDown();
}

std::pair<int, int> GameWindow::screenToWorld(int mouseX, int mouseY) {
  return camera.mouseToWorld(mouseX, mouseY);
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

void GameWindow::setMyPlayer(const ClientPlayer &player, uint32_t ID) {
  myPlayerID = ID;

  auto entity = std::make_unique<PlayerEntity>(player, *textureManager, *font);
  myPlayerEntity = entity.get();

  addEntity(EntityType::Player, ID, std::move(entity));
}

void GameWindow::addPlayer(uint32_t ID, const ClientPlayer &player) {
  if (ID == myPlayerID) {
    setMyPlayer(player, ID);
    return;
  }

  auto entity = std::make_unique<PlayerEntity>(player, *textureManager, *font);
  addEntity(EntityType::Player, ID, std::move(entity));
}

void GameWindow::renderInventoryPanel() {
  if (!invPanel || !myPlayerEntity)
    return;

  invPanel->render(myPlayerEntity->getPlayer());
}

void GameWindow::show(unsigned int it) {
  SDL_ClearError();
  clear();

  renderer->SetDrawColor(6, 8, 12, 255);
  renderer->FillRect(layout.windowRect);

  camera.setViewport(layout.gameRect.GetX(), layout.gameRect.GetY(),
                     layout.gameRect.GetW(), layout.gameRect.GetH());

  renderer->SetDrawColor(12, 16, 24, 255);
  renderer->FillRect(layout.gameRect);

  if (myPlayerEntity) {
    camera.follow(myPlayerEntity->get_x(), myPlayerEntity->get_y(),
                  ClientPlayer::Width, ClientPlayer::Height);
  }

  renderWorld(it);
  renderUIBackgrounds();
  renderHUD();
  renderUIFrame();

  renderer->Present();
}

void GameWindow::removePlayer(uint32_t ID) {
  removeEntity(EntityType::Player, ID);
}

void GameWindow::addCityEntity(uint32_t ID, CityEntityModel &entity,
                                CityEntityType entityType) {
    CityEntityInfo info = cityEntityParser.getInfo(entityType);
    auto renderable = std::make_unique<CityEntityRenderable>(
        entity, *textureManager, *font, info.textureId, info.layoutType,
        info.name);
    addEntity(EntityType::CityEntity, ID, std::move(renderable));
}

void GameWindow::removeCityEntity(uint32_t ID) {
    removeEntity(EntityType::CityEntity, ID);
}


void GameWindow::renderWorld(unsigned int it) {
  renderCommonGround();
  renderGroundItems();

  std::vector<RenderableEntity *> sortedEntities;
  getSortedEntities(sortedEntities);

  for (size_t i = 0; i < tilesToRender.size(); i++) {
    auto &priority = tilesToRender[i];

    for (auto &[pair, items] : priority) {
      int maxRow = pair.first;
      int yMax = (maxRow - maxSize / 2 + 1) * gridSize;

      for (auto *entity : sortedEntities) {
        if (!entity->rendered() &&
            entity->get_y() + entity->get_h() < yMax &&
            entity->hasPriority(i)) {
          entity->render(*renderer, camera, it);
        }
      }

      for (auto &item : items) {
        SDL2pp::Rect dstRect = camera.toScreen(
            (item.i - maxSize / 2) * gridSize,
            (item.j - maxSize / 2) * gridSize, gridSize, gridSize);

        SDL2pp::Rect srcRect = {
            item.x_start,
            item.y_start,
            item.x_end - item.x_start,
            item.y_end - item.y_start,
        };

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

  for (auto & entity : sortedEntities) {
    if (!entity->rendered()) {
      entity->render(*renderer, camera, it);
    }
  }
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

void GameWindow::renderHUD() {
  renderInventoryPanel();
  renderChat();
  renderPlayerHeader();
  renderPlayerStats();
  renderVitals();
}

void GameWindow::renderUIBackgrounds() {
  if (chatMessagesBackground) {
    renderer->Copy(*chatMessagesBackground, SDL2pp::NullOpt,
                   layout.chatMessagesRect);
  }

  if (chatInputBackground) {
    renderer->Copy(*chatInputBackground, SDL2pp::NullOpt,
                   layout.chatInputRect);
  }

  if (userInfoBackground) {
    renderer->Copy(*userInfoBackground, SDL2pp::NullOpt, layout.rightTopRect);
  }

  if (userInventoryBackground) {
    renderer->Copy(*userInventoryBackground, SDL2pp::NullOpt,
                   layout.inventoryRect);
  }

  if (userStatsBackground) {
    renderer->Copy(*userStatsBackground, SDL2pp::NullOpt,
                   layout.bottomRightRect);
  }
}

void GameWindow::renderUIFrame() {
  if (!uiFrameTexture)
    return;

  renderer->Copy(*uiFrameTexture, SDL2pp::NullOpt, layout.windowRect);
}

void GameWindow::renderChat() {
  if (!chatView)
    return;

  chatView->render(*renderer, layout.chatMessagesRect, layout.chatInputRect,
                   chatMessages, currentChatInput, chatActive);
}

void GameWindow::renderPlayerHeader() {
  if (!titleFont)
    return;

  SDL2pp::Surface surf =
      titleFont->RenderUTF8_Blended("Argentum", SDL_Color{255, 255, 255, 255});

  SDL2pp::Texture tex(*renderer, surf);

  int x = layout.rightTopRect.GetX() + 24 +
          (layout.rightTopRect.GetW() - surf.GetWidth()) / 2;

  int y = layout.rightTopRect.GetY() + 18;

  renderer->Copy(tex, SDL2pp::NullOpt,
                 SDL2pp::Rect(x, y, surf.GetWidth(), surf.GetHeight()));
}

void GameWindow::renderPlayerStats() {
  if (!myPlayerEntity)
    return;

  const ClientPlayer &p = myPlayerEntity->getPlayer();

  int xpCur = 357;  // placeholder, reemplazar por xp actual
  int xpMax = 1000; // placeholder, reemplazar por xp total para subir de nivel

  int x = layout.rightTopRect.GetX();
  int y = layout.rightTopRect.GetY();

  renderText(x + 40, y + 28, std::to_string(p.getLevel()),
             SDL_Color{255, 255, 200, 255});

  int xpX = x + 20;
  int xpY = y + 84;
  int xpW = 227;
  int xpH = 20;

  drawBar(xpX, xpY, xpW, xpH, xpCur, xpMax, SDL_Color{60, 180, 60, 255},
          SDL_Color{20, 20, 20, 255});

  SDL2pp::Rect xpBarRect(xpX, xpY, xpW, xpH);

  renderCenteredTextInRect(
      xpBarRect, std::to_string(xpCur) + " / " + std::to_string(xpMax),
      SDL_Color{255, 255, 255, 255});
}

void GameWindow::renderVitals() {
  if (!myPlayerEntity)
    return;

  const ClientPlayer &p = myPlayerEntity->getPlayer();

  int x = layout.bottomRightRect.GetX();
  int y = layout.bottomRightRect.GetY();

  int barX = x + 20;
  int barW = layout.bottomRightRect.GetW() - 40;
  int barH = 20;

  if (barW < 20)
    barW = 20;

  int hpY = y + 30;
  drawBar(barX, hpY, barW, barH, p.getHp(), p.getMaxHp(), kHpFill, kHpBg);

  SDL2pp::Rect hpBarRect(barX, hpY, barW, barH);
  renderCenteredTextInRect(
      hpBarRect,
      std::to_string(p.getHp()) + " / " + std::to_string(p.getMaxHp()),
      SDL_Color{255, 255, 255, 255});

  int manaY = y + 74;
  drawBar(barX, manaY, barW, barH, p.getMana(), p.getMaxMana(), kManaFill,
          kManaBg);

  SDL2pp::Rect manaBarRect(barX, manaY, barW, barH);
  renderCenteredTextInRect(
      manaBarRect,
      std::to_string(p.getMana()) + " / " + std::to_string(p.getMaxMana()),
      SDL_Color{255, 255, 255, 255});
}

void GameWindow::renderGroundItems() {
  for (const auto &[id, item] : groundItems) {
    SDL2pp::Texture *tex = textureManager->getItemIcon(item.itemId);
    if (!tex)
      continue;

    SDL2pp::Rect dst = camera.toScreen(item.x, item.y, 32, 32);
    renderer->Copy(*tex, SDL2pp::NullOpt, dst);
  }
}

void GameWindow::clear() {
  for (auto &[key, entity] : entities) {
    entity->clear();
  }
  renderer->Clear();
  renderer->SetDrawColor(0, 0, 0, 255);
  renderer->Clear();
}

void GameWindow::addNpc(uint32_t ID, NPC &npc, NPCType npcType) {
  NPCInfo info = npcParser.getInfo(npcType);
  auto entity = std::make_unique<NPCEntity>(npc, *textureManager,
                                            info.textureId, info.layoutType);
  addEntity(EntityType::Npc, ID, std::move(entity));
}

ClickTarget GameWindow::hitTestInventory(int screenX, int screenY) const {
  if (invPanel)
    return invPanel->handleClick(screenX, screenY);
  return {ClickTargetType::None, -1};
}

void GameWindow::updateGroundItems(
    const std::unordered_map<uint32_t, GroundItemInfoDTO> &items) {
  groundItems = items;
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
void GameWindow::renderText(int x, int y, const std::string &text,
                            SDL_Color color) {
  SDL2pp::Font *activeFont = uiFont ? uiFont.get() : font.get();

  if (!activeFont)
    return;

  SDL2pp::Surface surf = activeFont->RenderUTF8_Solid(text, color);
  SDL2pp::Texture tex(*renderer, surf);

  renderer->Copy(tex, SDL2pp::NullOpt,
                 SDL2pp::Rect(x, y, surf.GetWidth(), surf.GetHeight()));
}

void GameWindow::renderCenteredTextInRect(const SDL2pp::Rect &rect,
                                          const std::string &text,
                                          SDL_Color color) {
  if (!font)
    return;

  SDL2pp::Surface surf = font->RenderUTF8_Solid(text, color);
  SDL2pp::Texture tex(*renderer, surf);

  int x = rect.GetX() + (rect.GetW() - surf.GetWidth()) / 2;
  int y = rect.GetY() + (rect.GetH() - surf.GetHeight()) / 2;

  renderer->Copy(tex, SDL2pp::NullOpt,
                 SDL2pp::Rect(x, y, surf.GetWidth(), surf.GetHeight()));
}

void GameWindow::drawBar(int x, int y, int w, int h, uint32_t cur,
                         uint32_t max, SDL_Color fg, SDL_Color bg) {
  SDL2pp::Rect bgRect(x, y, w, h);

  renderer->SetDrawColor(bg.r, bg.g, bg.b, bg.a);
  renderer->FillRect(bgRect);

  if (max == 0)
    return;

  int fillW = static_cast<int>((static_cast<double>(cur) / max) * w);

  if (fillW <= 0)
    return;

  SDL2pp::Rect fillRect(x, y, fillW, h);

  renderer->SetDrawColor(fg.r, fg.g, fg.b, fg.a);
  renderer->FillRect(fillRect);
}
