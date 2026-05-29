#include "MapLoader.h"

#include <toml++/toml.hpp>

MapLoader::MapLoader(const std::string &path) {
  toml::table tbl = toml::parse_file(path);

  maxSize = tbl["MAX_SIZE"].value_or(100);
  gridSize = tbl["GRID_SIZE"].value_or(32);
  commonGroundTextureId = tbl["COMMON_GROUND_TEXTURE_ID"].value_or(0);

  if (auto *cellsArr = tbl["collidable_cells"].as_array()) {
    for (auto &elem : *cellsArr) {
      auto &cell = *elem.as_table();
      int i = cell["i"].value_or(0);
      int j = cell["j"].value_or(0);
      collidableCells.insert({i, j, 0});
    }
  }

  if (auto *texArr = tbl["textures"].as_array()) {
    for (auto &elem : *texArr) {
      auto &tex = *elem.as_table();
      TileOrigin origin;
      origin.priority = tex["priority"].value_or(0);
      origin.texture_id = tex["txt_id"].value_or(0);
      origin.x = tex["i"].value_or(0);
      origin.y = tex["j"].value_or(0);
      textureOrigins.push_back(std::move(origin));
    }
  }
}

int MapLoader::GetMaxSize() const { return maxSize; }

int MapLoader::GetGridSize() const { return gridSize; }

int MapLoader::GetCommonGroundTextureId() const {
  return commonGroundTextureId;
}

const std::set<std::tuple<int, int, int>> &MapLoader::GetCollidableCells()
    const {
  return collidableCells;
}

const std::list<TileOrigin> &MapLoader::GetTextureOrigins() const {
  return textureOrigins;
}
