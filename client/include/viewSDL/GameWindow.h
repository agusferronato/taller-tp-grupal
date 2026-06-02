#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2pp/SDL2pp.hh>
#include <cstdint>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Camera.h"
#include "EntityType.h"
#include "MapData.h"
#include "Player.h"
#include "RenderableEntity.h"
#include "TextureManager.h"
#include "TextureMapper.h"
#include "NPCParser.h"

class NPC;
class PlayerEntity;

class GameWindow {
private:
  SDL2pp::SDL sdl{SDL_INIT_VIDEO};
  SDL2pp::SDLImage sdlimage{IMG_INIT_PNG};
  SDL2pp::SDLTTF ttf;

  std::unique_ptr<SDL2pp::Window> window;
  std::unique_ptr<SDL2pp::Renderer> renderer;
  std::unique_ptr<SDL2pp::Font> font;

  std::unique_ptr<TextureManager> textureManager;

  Camera camera;
  uint32_t myPlayerID;

  using EntityKey = std::pair<EntityType, uint32_t>;
  std::map<EntityKey, std::unique_ptr<RenderableEntity>> entities;
  PlayerEntity *myPlayerEntity{nullptr};

  NPCParser npcParser;
  std::unique_ptr<TextureMapper> textureMapper;

  int maxSize, gridSize, commonGroundTextureId;
  int windowWidth, windowHeight;

  std::vector<std::map<std::pair<int, int>, std::vector<GridItem>>>
      tilesToRender;

public:
  GameWindow(uint32_t myPlayerID, int windowWidth, int windowHeight);

  void addEntity(EntityType type, uint32_t id,
                 std::unique_ptr<RenderableEntity> entity);
  void addPlayer(uint32_t ID, const Player &player);
  void addNpc(uint32_t ID, NPC &npc, NPCType npcType);
  void removeEntity(EntityType type, uint32_t id);
  void removePlayer(uint32_t ID);
  void setMyPlayer(const Player &player, uint32_t ID);

  SDL2pp::Renderer &getRenderer();
  SDL2pp::Font &getFont();
  void show(unsigned int it);
  void setMapData(int maxSize, int gridSize, int commonGroundTextureId,
                  const std::list<TileOrigin> &origins);

private:
  void renderHUD();
  void render(unsigned int it);
  void clear();
  void initResources();
  void renderCommonGround();

  void getSortedEntities(std::vector<RenderableEntity *> &);
};

#endif
