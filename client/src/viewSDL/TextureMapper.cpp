#include "TextureMapper.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>
#include <toml++/toml.hpp>

TextureMapper::TextureMapper(SDL2pp::Renderer &renderer) : renderer(renderer) {}

void TextureMapper::loadFromToml(const std::string &path) {
  toml::table tbl = toml::parse_file(path);
  if (auto *texTable = tbl["textures"].as_table()) {
    for (auto &[key, value] : *texTable) {
      auto &entry = *value.as_table();
      int id = entry["id"].value_or(0);
      std::string texPath = entry["path"].value_or("");
      bool transparent = entry["transparent"].value_or(false);

      SDL2pp::Surface surface(texPath);
      if (transparent) {
        surface.SetColorKey(true, SDL_MapRGB(surface.Get()->format, 0, 0, 0));
      }

      SDL2pp::Texture txt(renderer, surface);
      int w = txt.GetWidth();
      int h = txt.GetHeight();
      textures.emplace(id, TextureInfo{std::move(txt), w, h});
    }
  }
}

void TextureMapper::buildRenderGrid(const std::list<TileOrigin> &origins,
                                    int gridSizePx) {
  int maxPriority = 0;
  auto maxCmp = [](const auto &a, const auto &b) {
    return a.priority < b.priority;
  };
  auto itMax = std::max_element(origins.begin(), origins.end(), maxCmp);
  if (itMax != origins.end()) {
    maxPriority = itMax->priority;
  }
  tilesToRender.resize(maxPriority + 1);

  for (const auto &origin : origins) {
    auto it = textures.find(origin.texture_id);
    if (it == textures.end())
      continue;

    int rows = std::ceil(static_cast<float>(it->second.height) / gridSizePx);
    int columns = std::ceil(static_cast<float>(it->second.width) / gridSizePx);

    int spare_y = it->second.height;
    std::vector<GridItem> items;
    int max_row = 0, max_col = 0;

    for (int j = origin.y; j < origin.y + rows; j++) {
      int spare_x = it->second.width;

      for (int i = origin.x; i < origin.x + columns; i++) {
        GridItem tile;
        tile.texture_id = origin.texture_id;
        tile.x_start = (i - origin.x) * gridSizePx;
        tile.y_start = (j - origin.y) * gridSizePx;
        tile.x_end = tile.x_start + std::min(gridSizePx, spare_x);
        tile.y_end = tile.y_start + std::min(gridSizePx, spare_y);
        tile.i = i;
        tile.j = j;
        items.push_back(std::move(tile));
        max_col = i;
        spare_x -= gridSizePx;
      }

      spare_y -= gridSizePx;
      max_row = j;
    }

    auto key = std::make_pair(max_row, max_col);
    tilesToRender[origin.priority][key] = std::move(items);
  }
}

SDL2pp::Texture &TextureMapper::getTexture(int id) {
  return textures.at(id).texture;
}

const std::vector<std::map<std::pair<int, int>, std::vector<GridItem>>> &
TextureMapper::getTilesToRender() const {
  return tilesToRender;
}
