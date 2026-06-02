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
#include <vector>

#include "Camera.h"
#include "GameChatView.h"
#include "MapData.h"
#include "Player.h"
#include "TextureMapper.h"

class GameWindow {
private:

  // Struct auxiliar para organizar el UI
  struct Layout {
    SDL2pp::Rect windowRect;
    SDL2pp::Rect chatRect;
    SDL2pp::Rect gameRect;
    SDL2pp::Rect rightTopRect;
    SDL2pp::Rect inventoryRect;
    SDL2pp::Rect bottomRightRect;
  };

private:
  SDL2pp::SDL sdl{SDL_INIT_VIDEO};
  SDL2pp::SDLImage sdlimage{IMG_INIT_PNG};
  SDL2pp::SDLTTF ttf;

  std::unique_ptr<SDL2pp::Window> window;
  std::unique_ptr<SDL2pp::Renderer> renderer;

  std::unique_ptr<SDL2pp::Texture> uiFrameTexture;
  std::unique_ptr<SDL2pp::Font> font;
  std::unique_ptr<GameChatView> chatView;
  std::unique_ptr<TextureMapper> textureMapper;

  // Chat state
  std::deque<std::string> chatMessages;
  std::string currentChatInput;
  bool chatActive{false};

  Camera camera;

  uint32_t myPlayerID{0};

  std::unordered_map<uint32_t, Player *> players;
  std::list<RenderableEntity *> entities;

  int maxSize{100};
  int gridSize{32};
  int commonGroundTextureId{0};

  std::vector<std::map<std::pair<int, int>, std::vector<GridItem>>>
      tilesToRender;

private:
  std::string headPathForRace(const std::string &race) const;

  Layout getLayout() const;

  void initResources();
  void clear();

  void render(unsigned int it);
  void renderWorld(unsigned int it);
  void renderCommonGround();
  void renderEntitiesByPriority(unsigned int it);

  void renderHUD();
  void renderUIFrame(const Layout &layout);
  void renderChat(const Layout &layout);
  void renderPlayerStats(const Layout &layout);

  void renderText(int x, int y,
                  const std::string &text,
                  SDL_Color color);

  void drawBar(int x, int y,
               int w, int h,
               uint32_t cur,
               uint32_t max,
               SDL_Color fg,
               SDL_Color bg);

  std::unique_ptr<SDL2pp::Texture>
  loadPlayerTexture(SDL2pp::Renderer &renderer,
                    const std::string &texturePath);

public:
  explicit GameWindow(uint32_t myPlayerID);

  void show(unsigned int it);

  void addPlayer(uint32_t id, Player *player);
  void removePlayer(uint32_t id);

  void setMapData(int maxSize,
                  int gridSize,
                  int commonGroundTextureId,
                  const std::list<TileOrigin> &origins);

  void setChatState(const std::deque<std::string> &messages,
                    const std::string &input,
                    bool active);
};

#endif