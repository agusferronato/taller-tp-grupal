#include "Grid.h"

Grid::Grid(Camera &camera, SDL2pp::Renderer &renderer)
    : tilesToRender(
          std::vector<std::map<std::pair<int, int>, std::vector<GridItem>>>(3)),
      camera(camera), downloader("map.toml"), font("fonts/Timeless.ttf", 16),
      colissionTexture(renderer, "assets/colision.png") {}

void Grid::setGridTexture(TextureMap &textureMap, int texture_id) {

  if (thereAreAssignedTextures(textureMap, texture_id))
    return;

  TextureInMap &txtInMap = textureMap.getTexture(texture_id);
  SDL2pp::Texture &txt = txtInMap.txt;

  int rows = std::ceil((float)txt.GetHeight() / GRID_SIZE_PX);
  int columns = std::ceil((float)txt.GetWidth() / GRID_SIZE_PX);

  int spare_y = txt.GetHeight();

  txtOrigins.push_back(
      {txtInMap.data.priority, texture_id, item_hover_i, item_hover_j});

  float collidablePercentage = txtInMap.data.collidablePercentage;
  int offset = std::floor((1 - collidablePercentage) * rows);

  std::vector<GridItem> gridItemList;

  int max_row, max_col;

  for (int j = item_hover_j; j < item_hover_j + rows; j++) {

    int spare_x = txt.GetWidth();

    for (int i = item_hover_i; i < item_hover_i + columns; i++) {

      GridItem tile;

      tile.texture_id = texture_id;

      tile.x_start = (i - item_hover_i) * GRID_SIZE_PX;
      tile.y_start = (j - item_hover_j) * GRID_SIZE_PX;

      tile.x_end = tile.x_start + std::min(GRID_SIZE_PX, spare_x);
      tile.y_end = tile.y_start + std::min(GRID_SIZE_PX, spare_y);

      tile.i = i;
      tile.j = j;

      gridItemList.push_back(std::move(tile));

      SDL2pp::Rect rect = {tile.x_start, tile.y_start,
                           tile.x_end - tile.x_start,
                           tile.y_end - tile.y_start};

      if (j >= item_hover_j + offset &&
          getAlphaChannelWeight(txtInMap.surface, rect) > 0.15) {
        collidableCells.insert({i, j, txtInMap.data.priority});
      }

      max_col = i;
      spare_x -= GRID_SIZE_PX;
    }

    spare_y -= GRID_SIZE_PX;

    max_row = j;
  }

  auto key = std::make_pair(max_row, max_col);
  tilesToRender[txtInMap.data.priority][key] = std::move(gridItemList);
}

bool Grid::thereAreAssignedTextures(TextureMap &textureMap, int texture_id) {

  TextureInMap &txtInMap = textureMap.getTexture(texture_id);
  SDL2pp::Texture &txt = txtInMap.txt;

  int rows = std::ceil((float)txt.GetHeight() / GRID_SIZE_PX);
  int columns = std::ceil((float)txt.GetWidth() / GRID_SIZE_PX);

  for (int j = item_hover_j; j < item_hover_j + rows; j++) {

    for (int i = item_hover_i; i < item_hover_i + columns; i++) {

      if (collidableCells.find({i, j, txtInMap.data.priority}) !=
          collidableCells.end())
        return true;
    }
  }

  return false;
}

float Grid::getAlphaChannelWeight(SDL2pp::Surface &surface,
                                  SDL2pp::Rect region) {

  SDL2pp::Surface converted = surface.Convert(SDL_PIXELFORMAT_RGBA8888);

  SDL2pp::Surface::LockHandle lock = converted.Lock();

  long long totalPixels = region.w * region.h;
  long long alpha = 0;

  int pitch = converted.Get()->pitch / sizeof(Uint32);
  Uint32 *pixels = static_cast<Uint32 *>(converted.Get()->pixels);

  for (int y = region.y; y < region.y + region.h; y++) {
    for (int x = region.x; x < region.x + region.w; x++) {

      Uint32 pixel = pixels[y * pitch + x];

      Uint8 r, g, b, a;
      SDL_GetRGBA(pixel, converted.Get()->format, &r, &g, &b, &a);

      alpha += a;
    }
  }

  return static_cast<float>(alpha) / (totalPixels * 255.0f);
}

void Grid::render(SDL2pp::Renderer &renderer, TextureMap &textureMap) {

  renderCommonGround(renderer, textureMap);

  for (auto &priority : tilesToRender) {

    for (auto &[_, items] : priority) {

      for (auto &item : items) {

        SDL2pp::Rect dstRect = camera.toScreen(
            (item.i - MAX_SIZE / 2) * GRID_SIZE_PX,
            (item.j - MAX_SIZE / 2) * GRID_SIZE_PX, GRID_SIZE_PX, GRID_SIZE_PX);

        SDL2pp::Rect srcRect = {item.x_start, item.y_start,
                                item.x_end - item.x_start,
                                item.y_end - item.y_start};

        renderer.Copy(textureMap.getTexture(item.texture_id).txt, srcRect,
                      dstRect);
      }
    }
  }

  updateSelectedBiome();
  renderBiomes(renderer);

  if (mustShowcollidableCells)
    renderCollidableCells(renderer);

  renderHover(renderer);
}

void Grid::renderCollidableCells(SDL2pp::Renderer &renderer) {

  colissionTexture.SetAlphaMod(80);

  for (auto &[i, j, _] : collidableCells) {

    SDL2pp::Rect dstRect = camera.toScreen((i - MAX_SIZE / 2) * GRID_SIZE_PX,
                                           (j - MAX_SIZE / 2) * GRID_SIZE_PX,
                                           GRID_SIZE_PX, GRID_SIZE_PX);

    SDL2pp::Rect srcRect = {0, 0, GRID_SIZE_PX, GRID_SIZE_PX};
    renderer.Copy(colissionTexture, srcRect, dstRect);
  }

  colissionTexture.SetAlphaMod(255);
}

void Grid::changeCollidableCellsVisibility() {
  mustShowcollidableCells = !mustShowcollidableCells;
}

void Grid::renderCommonGround(SDL2pp::Renderer &renderer,
                              TextureMap &textureMap) {

  for (int i = 0; i < MAX_SIZE; i++) {
    for (int j = 0; j < MAX_SIZE; j++) {
      SDL2pp::Rect dstRect = camera.toScreen((i - MAX_SIZE / 2) * GRID_SIZE_PX,
                                             (j - MAX_SIZE / 2) * GRID_SIZE_PX,
                                             GRID_SIZE_PX, GRID_SIZE_PX);

      SDL2pp::Rect srcRect = {0, 0, GRID_SIZE_PX, GRID_SIZE_PX};
      renderer.Copy(textureMap.getTexture(GRASS_TEXTURE_ID).txt, srcRect,
                    dstRect);
    }
  }
}

void Grid::renderHover(SDL2pp::Renderer &renderer) {

  SDL2pp::Rect dstRect = camera.toScreen(
      (item_hover_i - MAX_SIZE / 2) * GRID_SIZE_PX,
      (item_hover_j - MAX_SIZE / 2) * GRID_SIZE_PX, GRID_SIZE_PX, GRID_SIZE_PX);

  SDL_SetRenderDrawBlendMode(renderer.Get(), SDL_BLENDMODE_BLEND);
  renderer.SetDrawColor(255, 100, 0, 50);
  renderer.FillRect(dstRect);
}

void Grid::setMousePosition(int x, int y) {
  hover_init = true;
  int worldX = camera.get_x() + x;
  int worldY = camera.get_y() + y;

  item_hover_i = (int)std::floor((float)worldX / GRID_SIZE_PX) + MAX_SIZE / 2;
  item_hover_j = (int)std::floor((float)worldY / GRID_SIZE_PX) + MAX_SIZE / 2;

  item_hover_i = std::clamp(item_hover_i, 0, MAX_SIZE - 1);
  item_hover_j = std::clamp(item_hover_j, 0, MAX_SIZE - 1);
}

void Grid::saveMap(GridSDL &gridSDL) {
  this->downloader.saveMap(gridSDL, txtOrigins, collidableCells, biomes);
}

void Grid::setInitBiomePosition(Biome biome) {

  biomeSelected = true;

  biomes.push_back(BiomeGrid{
      biome,
      biomeParser.getBiomeAsString(biome),
      biomeParser.getBiomeColor(biome),
      false,
      item_hover_i,
      item_hover_j,
      item_hover_i,
      item_hover_j,
      item_hover_i,
      item_hover_j,
  });
}

void Grid::releaseBiomeSelection() {
  biomeSelected = false;
  biomes[biomes.size() - 1].initialized = true;
}

void Grid::updateSelectedBiome() {

  if (!biomeSelected)
    return;

  BiomeGrid &biomeSelectedGrid = biomes[biomes.size() - 1];

  biomeSelectedGrid.i_init = std::min(biomeSelectedGrid.i_start, item_hover_i);
  biomeSelectedGrid.i_end = std::max(biomeSelectedGrid.i_start, item_hover_i);
  biomeSelectedGrid.j_init = std::min(biomeSelectedGrid.j_start, item_hover_j);
  biomeSelectedGrid.j_end = std::max(biomeSelectedGrid.j_start, item_hover_j);
}

void Grid::renderBiomes(SDL2pp::Renderer &renderer) {

  for (auto &biome : biomes) {

    for (int i = biome.i_init; i <= biome.i_end; i++) {
      for (int j = biome.j_init; j <= biome.j_end; j++) {

        SDL2pp::Rect dstRect = camera.toScreen(
            (i - MAX_SIZE / 2) * GRID_SIZE_PX,
            (j - MAX_SIZE / 2) * GRID_SIZE_PX, GRID_SIZE_PX, GRID_SIZE_PX);

        SDL2pp::Color &color = biome.color;

        SDL_SetRenderDrawBlendMode(renderer.Get(), SDL_BLENDMODE_BLEND);
        renderer.SetDrawColor(color.r, color.g, color.b, color.a);
        renderer.FillRect(dstRect);
      }
    }

    if (biome.initialized) {

      SDL2pp::Rect cornerRect = camera.toScreen(
          (biome.i_init - MAX_SIZE / 2) * GRID_SIZE_PX,
          (biome.j_end + 1 - MAX_SIZE / 2) * GRID_SIZE_PX, 0, 0);

      SDL2pp::Surface textSurface(TTF_RenderText_Blended(
          font.Get(), biome.asString.c_str(), {255, 255, 255, 255}));
      SDL2pp::Texture textTexture(renderer, textSurface);

      int tw = textTexture.GetWidth();
      int th = textTexture.GetHeight();

      renderer.Copy(
          textTexture, SDL2pp::NullOpt,
          SDL2pp::Rect(cornerRect.x + 16, cornerRect.y - th + 32, tw, th));
    }
  }
}
