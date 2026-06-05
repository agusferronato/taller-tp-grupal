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
#include <memory>
#include <vector>

#define MAX_SIZE 100
#define GRID_SIZE_PX 32
#include "Biome.h"
#include "BiomeDataParser.h"
#include "Info.h"
#include "Tile.h"

class GridSDL;


struct TileOrigin {
  int priority;
  int texture_id;
  int x;
  int y;
};

struct BiomeGrid {
    int id; 
    Biome type;
    std::string asString;
    SDL2pp::Color color;
    bool initialized{false};
    int i_start, j_start;
    int i_init, j_init;
    int i_end, j_end;
};

class Grid {

private:
  int size{MAX_SIZE};

  std::list<TileOrigin> txtOrigins;

  std::set<std::tuple<int, int, int>> collidableCells;

  std::vector<std::map<
    std::pair<int, int>, 
    std::shared_ptr<Tile>>
  > tilesToRender;

  std::map<int, BiomeGrid> biomes;

  Camera &camera;
  MapDownloader downloader;
  SDL2pp::Font font;
  SDL2pp::Texture colissionTexture;

  BiomeDataParser biomeParser;

  int active_texture_id{0};
  std::shared_ptr<Tile> hoverTile{nullptr};
  std::shared_ptr<Tile> selectedTile{nullptr}; 
  int selectedBiomeId{-1};

  int item_hover_i, item_hover_j;
  bool isPlacingBiome{false};
  int currentBiomePlacingId{-1};
  bool mustShowcollidableCells{false};

  int next_instance_id{0};

public:
  Grid(Camera &camera, SDL2pp::Renderer &renderer);

  void setHoverTexture(TextureMap &textureMap, int texture_id);
  void clearHoverTexture();
  void tryPlaceHoverTexture();

  void render(SDL2pp::Renderer &renderer, TextureMap &textureMap);

  void setMousePosition(int x, int y);

  void saveMap(GridSDL &gridSDL);

  void setInitBiomePosition(Biome biome);

  void releaseBiomeSelection();
  void changeCollidableCellsVisibility();

  bool selectElementAt(int i, int j); 
  void deleteSelectedTexture();
  void startMovingSelectedTexture();
  void deleteSelectedBiome();

  void renderHoverAndSelection(SDL2pp::Renderer &renderer);

  std::shared_ptr<Tile> getSelectedTile() const { return selectedTile; }
  int getSelectedBiomeId() const { return selectedBiomeId; }

  std::map<int, BiomeGrid> &getBiomes();


private:

  std::shared_ptr<Tile> createTileInstance(TextureMap &textureMap, int texture_id, int start_i, int start_j);
  bool checkCollisions(const std::shared_ptr<Tile> &tile);

  void updateSelectedBiome();

  void renderBiomes(SDL2pp::Renderer &renderer);
  bool thereAreAssignedTextures(TextureMap &textureMap, int texture_id);

  void renderCollidableCells(SDL2pp::Renderer &renderer);

  float getAlphaChannelWeight(SDL2pp::Surface &surface, SDL2pp::Rect region);

  void renderHover(SDL2pp::Renderer &renderer);
  void renderCommonGround(SDL2pp::Renderer &renderer, TextureMap &textureMap);
};

#endif
