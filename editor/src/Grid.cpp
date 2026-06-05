#include "Grid.h"

Grid::Grid(Camera &camera, SDL2pp::Renderer &renderer)
    : tilesToRender(
          std::vector<std::map<std::pair<int, int>, std::shared_ptr<Tile>>>(3)),
      camera(camera), downloader("map.toml"), font("fonts/Timeless.ttf", 16),
      colissionTexture(renderer, "assets/colision.png") {}


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

    for (auto &[_, tile] : priority) {

      for (auto &item : tile->getItems()) {

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

    int worldX = camera.get_x() + x;
    int worldY = camera.get_y() + y;

    item_hover_i = (int)std::floor((float)worldX / GRID_SIZE_PX) + MAX_SIZE / 2;
    item_hover_j = (int)std::floor((float)worldY / GRID_SIZE_PX) + MAX_SIZE / 2;

    item_hover_i = std::clamp(item_hover_i, 0, MAX_SIZE - 1);
    item_hover_j = std::clamp(item_hover_j, 0, MAX_SIZE - 1); 

    if (hoverTile) {
        int p = hoverTile->getPriority();
        std::pair<int, int> oldKey = {hoverTile->getMaxI(), hoverTile->getMaxJ()};

        tilesToRender[p].erase(oldKey);

        hoverTile->updatePosition(item_hover_i, item_hover_j);

        std::pair<int, int> newKey = {hoverTile->getMaxI(), hoverTile->getMaxJ()};
        tilesToRender[p].insert({newKey, hoverTile});
    }

}

void Grid::saveMap(GridSDL &gridSDL) {
  this->downloader.saveMap(gridSDL, txtOrigins, collidableCells, biomes);
}


void Grid::setInitBiomePosition(Biome biome) {
    isPlacingBiome = true;
    currentBiomePlacingId = next_instance_id++;
    
    biomes[currentBiomePlacingId] = BiomeGrid{
        currentBiomePlacingId, biome, biomeParser.getBiomeAsString(biome), biomeParser.getBiomeColor(biome),
        false, item_hover_i, item_hover_j, item_hover_i, item_hover_j, item_hover_i, item_hover_j
    };
}

void Grid::releaseBiomeSelection() {
    if (isPlacingBiome) {
        isPlacingBiome = false;
        biomes[currentBiomePlacingId].initialized = true;
    }
}

void Grid::updateSelectedBiome() {

  if (!isPlacingBiome)
    return;

  biomes[currentBiomePlacingId].i_init = std::min(biomes[currentBiomePlacingId].i_start, item_hover_i);
  biomes[currentBiomePlacingId].i_end = std::max(biomes[currentBiomePlacingId].i_start, item_hover_i);
  biomes[currentBiomePlacingId].j_init = std::min(biomes[currentBiomePlacingId].j_start, item_hover_j);
  biomes[currentBiomePlacingId].j_end = std::max(biomes[currentBiomePlacingId].j_start, item_hover_j);
}



void Grid::renderBiomes(SDL2pp::Renderer &renderer) {

  for (auto &[_, biome] : biomes) {

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



std::shared_ptr<Tile> Grid::createTileInstance(TextureMap &textureMap, int texture_id, int start_i, int start_j) {
    TextureInMap &txtInMap = textureMap.getTexture(texture_id);
    SDL2pp::Texture &txt = txtInMap.txt;

    int rows = std::ceil((float)txt.GetHeight() / GRID_SIZE_PX);
    int columns = std::ceil((float)txt.GetWidth() / GRID_SIZE_PX);

    auto tile = std::make_shared<Tile>(-1, texture_id, txtInMap.data.priority, start_i, start_j, txt.GetWidth(), txt.GetHeight());

    int spare_y = txt.GetHeight();
    float collidablePercentage = txtInMap.data.collidablePercentage;
    int offset = std::floor((1 - collidablePercentage) * rows);

    for (int j = start_j; j < start_j + rows; j++) {
        int spare_x = txt.GetWidth();
        for (int i = start_i; i < start_i + columns; i++) {
            GridItem item;
            item.texture_id = texture_id;
            item.x_start = (i - start_i) * GRID_SIZE_PX;
            item.y_start = (j - start_j) * GRID_SIZE_PX;
            item.x_end = item.x_start + std::min(GRID_SIZE_PX, spare_x);
            item.y_end = item.y_start + std::min(GRID_SIZE_PX, spare_y);
            item.i = i;
            item.j = j;
            tile->addItem(item);

            SDL2pp::Rect rect = {item.x_start, item.y_start, item.x_end - item.x_start, item.y_end - item.y_start};
            if (j >= start_j + offset && getAlphaChannelWeight(txtInMap.surface, rect) > 0.15) {
                tile->addCollidable(i, j);
            }
            spare_x -= GRID_SIZE_PX;
        }
        spare_y -= GRID_SIZE_PX;
    }
    return tile;
}


std::map<int, BiomeGrid>& Grid::getBiomes() {
    return biomes;
}


void Grid::setHoverTexture(TextureMap &textureMap, int texture_id) {
    active_texture_id = texture_id;
    hoverTile = createTileInstance(textureMap, texture_id, item_hover_i, item_hover_j);
}

void Grid::clearHoverTexture() {
    hoverTile = nullptr;
    active_texture_id = 0;
} 


bool Grid::checkCollisions(const std::shared_ptr<Tile>& tile) {

    if (!tile) return false;

    std::map<std::pair<int, int>, std::shared_ptr<Tile>>& tiles = tilesToRender[tile->getPriority()];  

    for (const auto& [_, placedTile] : tiles) {

        for (const auto& cell : tile->getCollidableCells()) {
            for (const auto& placedCell : placedTile->getCollidableCells()) {
                if (
                    std::get<0>(cell) == std::get<0>(placedCell) 
                    && std::get<1>(cell) == std::get<1>(placedCell)
                ) 
                    return true; 
            }
        }
    }
    return false;

}

void Grid::tryPlaceHoverTexture() {
    if (!hoverTile || checkCollisions(hoverTile)) return;

    txtOrigins.push_back({
        hoverTile->getPriority(), 
        hoverTile->getTextureId(), 
        hoverTile->getOriginI(), 
        hoverTile->getOriginY()
    });

    for(auto cell : hoverTile->getCollidableCells()) {
        collidableCells.insert(cell);
    }

    hoverTile = nullptr; 
    active_texture_id = 0;
}


bool Grid::selectElementAt(int i, int j) {

    selectedTile = nullptr;
    selectedBiomeId = -1;

    for (int p = tilesToRender.size() - 1; p >= 0; --p) {
        for (auto const& [_, tile] : tilesToRender[p]) {

            for (const auto& item : tile->getItems()) {
                if (item.i == i && item.j == j) {
                    selectedTile = tile;
                    return true;
                }
            }
        }
    }

    for (auto const& [id, biome] : biomes) {
        if (biome.initialized && i >= biome.i_init && i <= biome.i_end && j >= biome.j_init && j <= biome.j_end) {
            selectedBiomeId = id;
            return true;
        }
    }

    return false;
}


void Grid::deleteSelectedTexture() {
    if (selectedTile) {

        txtOrigins.remove_if([this](const TileOrigin& origin) {
            return origin.x == selectedTile->getOriginI() && 
                   origin.y == selectedTile->getOriginY() &&
                   origin.texture_id == selectedTile->getTextureId();
        });

        for (const auto& cell : selectedTile->getCollidableCells()) {
            collidableCells.erase(cell);
        }

        int p = selectedTile->getPriority();
        std::pair<int, int> key = {selectedTile->getMaxJ(), selectedTile->getMaxI()};
        
        auto it = tilesToRender[p].find(key);
        if (it != tilesToRender[p].end() && it->second->getId() == selectedTile->getId()) {
            tilesToRender[p].erase(it);
        }
        selectedTile = nullptr;

    }
}

void Grid::startMovingSelectedTexture() {

    if (selectedTile) {

        hoverTile = selectedTile;
        active_texture_id = selectedTile->getTextureId();

        txtOrigins.remove_if([this](const TileOrigin& origin) {
            return origin.x == selectedTile->getOriginI() && 
                   origin.y == selectedTile->getOriginY() &&
                   origin.texture_id == selectedTile->getTextureId();
        });

        for(auto cell : selectedTile->getCollidableCells()) {
            collidableCells.erase(cell);
        }

        selectedTile = nullptr;
    }

}

void Grid::deleteSelectedBiome() {
    if (selectedBiomeId != -1) {
        biomes.erase(selectedBiomeId);
        selectedBiomeId = -1;
    }
}


void Grid::renderHoverAndSelection(SDL2pp::Renderer &renderer) {

    if (selectedTile) {
        int min_i = selectedTile->getMinI();
        int min_j = selectedTile->getMinJ();
        int max_i = selectedTile->getMaxI();
        int max_j = selectedTile->getMaxJ();

        SDL2pp::Rect dstRect = camera.toScreen(
            (selectedTile->getMinI() - MAX_SIZE / 2) * GRID_SIZE_PX,
            (selectedTile->getMinJ() - MAX_SIZE / 2) * GRID_SIZE_PX,
            (max_i - min_i + 1) * GRID_SIZE_PX,
            (max_j - min_j + 1) * GRID_SIZE_PX
        );
        SDL_SetRenderDrawBlendMode(renderer.Get(), SDL_BLENDMODE_BLEND);
        renderer.SetDrawColor(255, 140, 0, 255);
        renderer.DrawRect(dstRect);
    }

}