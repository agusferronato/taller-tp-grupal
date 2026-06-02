#ifndef GRID_H
#define GRID_H

#include "Camera.h"
#include "MapDownloader.h"
#include "TextureMap.h"
#include <SDL2pp/SDL2pp.hh>
#include <array>
#include <list>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#define MAX_SIZE 100
#define GRID_SIZE_PX 32
#include "Biome.h"
#include "BiomeDataParser.h"
#include "Info.h"

class GridSDL;

struct GridItem {

  int texture_id{GRASS_TEXTURE_ID};

  int x_end{GRID_SIZE_PX};
  int y_end{GRID_SIZE_PX};
  int x_start{0};
  int y_start{0};

  int i, j;

  bool isCollidable{false};
};

struct TileOrigin {
  int priority;
  int texture_id;
  int x;
  int y;
};

struct BiomeGrid {

  Biome type;
  std::string asString;
  SDL2pp::Color color;

  bool initialized{false};

  int i_start;
  int j_start;

  int i_init;
  int j_init;
  int i_end;
  int j_end;
};

class Grid {

private:
  int size{MAX_SIZE};

  std::list<TileOrigin> txtOrigins;

  std::set<std::tuple<int, int, int>> collidableCells;

  std::vector<std::map<std::pair<int, int>, std::vector<GridItem>>>
      tilesToRender;

  std::vector<BiomeGrid> biomes;

  Camera &camera;
  MapDownloader downloader;
  SDL2pp::Font font;
  SDL2pp::Texture colissionTexture;

  BiomeDataParser biomeParser;

  int item_hover_i, item_hover_j;
  bool hover_init{false};
  bool biomeSelected{false};
  bool mustShowcollidableCells{false};

public:
  Grid(Camera &camera, SDL2pp::Renderer &renderer);

  void setGridTexture(TextureMap &textureMap, int texture_id);

  void render(SDL2pp::Renderer &renderer, TextureMap &textureMap);

  void setMousePosition(int x, int y);

  void saveMap(GridSDL &gridSDL);

  void setInitBiomePosition(Biome biome);

  void releaseBiomeSelection();
  void changeCollidableCellsVisibility();

private:
  void updateSelectedBiome();

  void renderBiomes(SDL2pp::Renderer &renderer);
  bool thereAreAssignedTextures(TextureMap &textureMap, int texture_id);

  void renderCollidableCells(SDL2pp::Renderer &renderer);

  float getAlphaChannelWeight(SDL2pp::Surface &surface, SDL2pp::Rect region);

  void renderHover(SDL2pp::Renderer &renderer);
  void renderCommonGround(SDL2pp::Renderer &renderer, TextureMap &textureMap);
};

#endif
