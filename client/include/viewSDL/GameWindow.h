#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2pp/SDL2pp.hh>
#include <cstdint>
#include <deque>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "BloodOverlay.h"
#include "Camera.h"
#include "ChatMessage.h"
#include "ClientPlayer.h"
#include "EntityType.h"
#include "GameChatView.h"
#include "GroundItemsListEventDTO.h"
#include "InventoryPanel.h"

#include "MapData.h"
#include "CityEntityParser.h"
#include "NPCParser.h"
#include "Player.h"
#include "RenderableEntity.h"
#include "TextureManager.h"
#include "TextureMapper.h"

class CityEntityModel;
class NPC;
class PlayerEntity;

class GameWindow {
private:

  struct Layout {
    SDL2pp::Rect windowRect;
    SDL2pp::Rect chatMessagesRect;
    SDL2pp::Rect chatInputRect;
    SDL2pp::Rect gameRect;
    SDL2pp::Rect rightTopRect;
    SDL2pp::Rect inventoryRect;
    SDL2pp::Rect bottomRightRect;
  };

  const Layout layout{
    // Pantalla completa
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
    SDL2pp::Rect(686, 429, 267, 104),
  };

  SDL2pp::SDL sdl{SDL_INIT_VIDEO};
  SDL2pp::SDLImage sdlimage{IMG_INIT_PNG};
  SDL2pp::SDLTTF ttf;

  std::unique_ptr<SDL2pp::Window> window;
  std::unique_ptr<SDL2pp::Renderer> renderer;

  std::unique_ptr<SDL2pp::Texture> uiFrameTexture;
  std::unique_ptr<GameChatView> chatView;
  std::unique_ptr<SDL2pp::Font> font;
  std::unique_ptr<SDL2pp::Font> titleFont;
  std::unique_ptr<SDL2pp::Font> uiFont;
  std::unique_ptr<SDL2pp::Texture> chatMessagesBackground;
  std::unique_ptr<SDL2pp::Texture> chatInputBackground;
  std::unique_ptr<SDL2pp::Texture> userInfoBackground;
  std::unique_ptr<SDL2pp::Texture> userInventoryBackground;
  std::unique_ptr<SDL2pp::Texture> userStatsBackground;

  std::unique_ptr<TextureManager> textureManager;
  std::unique_ptr<BloodOverlay> bloodOverlay;

  Camera camera;
  uint32_t myPlayerID;

  std::deque<ChatMessage> chatMessages;
  std::string currentChatInput;
  bool chatActive{false};

  using EntityKey = std::pair<EntityType, uint32_t>;
  std::map<EntityKey, std::unique_ptr<RenderableEntity>> entities;
  PlayerEntity *myPlayerEntity{nullptr};

  NPCParser npcParser;
  CityEntityParser cityEntityParser;
  std::unique_ptr<TextureMapper> textureMapper;
  std::unique_ptr<InventoryPanel> invPanel;

  std::unordered_map<uint32_t, GroundItemInfoDTO> groundItems;

  int maxSize, gridSize, commonGroundTextureId;
  int windowWidth, windowHeight;

  std::vector<std::map<std::pair<int, int>, std::vector<GridItem>>>
      tilesToRender;

public:
  explicit GameWindow(uint32_t myPlayerID);

  void addEntity(EntityType type, uint32_t id,
                 std::unique_ptr<RenderableEntity> entity);
  void addPlayer(uint32_t ID, const ClientPlayer &player);
  void addNpc(uint32_t ID, NPC &npc, NPCType npcType);
  void addCityEntity(uint32_t ID, CityEntityModel &entity, CityEntityType entityType);
  void removeCityEntity(uint32_t ID);
  void removeEntity(EntityType type, uint32_t id);
  void removePlayer(uint32_t ID);
  void setMyPlayer(const ClientPlayer &player, uint32_t ID);

  SDL2pp::Renderer &getRenderer();
  SDL2pp::Font &getFont();
  void show(unsigned int it);

  void setMapData(int maxSize, int gridSize, int commonGroundTextureId,
                  const std::list<TileOrigin> &origins);

  ClickTarget hitTestInventory(int screenX, int screenY) const;
  void updateGroundItems(const std::unordered_map<uint32_t, GroundItemInfoDTO> &items);
  void setChatState(const std::deque<ChatMessage> &messages,
                    const std::string &input, bool active);

  void scrollChatUp();
  void scrollChatDown();
  std::pair<int, int> screenToWorld(int mouseX, int mouseY);
  void zoomOutCamera();
  void resetCameraZoom();

private:
  void renderHUD();
  void renderWorld(unsigned int it);
  void clear();
  void renderChat();
  void renderUIBackgrounds();
  void renderUIFrame();
  void renderPlayerStats();
  void renderPlayerHeader();
  void renderVitals();
  void renderInventoryPanel();
  void initResources();
  void renderCommonGround();
  void renderGroundItems();

  void getSortedEntities(std::vector<RenderableEntity *> &);
  void renderText(int x, int y, const std::string &text, SDL_Color color);
  void renderCenteredTextInRect(const SDL2pp::Rect &rect,
                                const std::string &text, SDL_Color color);
  void drawBar(int x, int y, int w, int h, uint32_t cur, uint32_t max,
               SDL_Color fg, SDL_Color bg);
};

#endif
