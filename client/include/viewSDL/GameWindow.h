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

#include "Camera.h"
#include "ClientPlayer.h"
#include "EntityType.h"
#include "GameChatView.h"
#include "GroundItemsListEventDTO.h"
#include "InventoryPanel.h"

#include "MapData.h"
#include "NPCParser.h"
#include "Player.h"
#include "RenderableEntity.h"
#include "TextureManager.h"
#include "TextureMapper.h"

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

  Camera camera;
  uint32_t myPlayerID;

  std::deque<std::string> chatMessages;
  std::string currentChatInput;
  bool chatActive{false};

  using EntityKey = std::pair<EntityType, uint32_t>;
  std::map<EntityKey, std::unique_ptr<RenderableEntity>> entities;
  PlayerEntity *myPlayerEntity{nullptr};

  NPCParser npcParser;
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
  void setChatState(const std::deque<std::string> &messages,
                    const std::string &input, bool active);

private:
  void renderHUD(const Layout &layout);
  void renderWorld(unsigned int it);
  void clear();
  void renderChat(const Layout &layout);
  void renderUIBackgrounds(const Layout &layout);
  void renderUIFrame(const Layout &layout);
  void renderPlayerStats(const Layout &layout);
  void renderPlayerHeader(const Layout &layout);
  void renderVitals(const Layout &layout);
  void renderInventoryPanel(const Layout &layout);
  void initResources();
  void renderCommonGround();
  void renderGroundItems();

  void getSortedEntities(std::vector<RenderableEntity *> &);
  Layout getLayout() const;
  void renderText(int x, int y, const std::string &text, SDL_Color color);
  void renderCenteredTextInRect(const SDL2pp::Rect &rect,
                                const std::string &text, SDL_Color color);
  void drawBar(int x, int y, int w, int h, uint32_t cur, uint32_t max,
               SDL_Color fg, SDL_Color bg);
};

#endif
