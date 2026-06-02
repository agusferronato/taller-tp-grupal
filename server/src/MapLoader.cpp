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

  if (auto *bioArr = tbl["biomes"].as_array()) {
    for (auto &elem : *bioArr) {
      auto &b = *elem.as_table();
      std::string type = b["type"].value_or("");
      int i_init = b["i_init"].value_or(0);
      int j_init = b["j_init"].value_or(0);
      int i_end = b["i_end"].value_or(0);
      int j_end = b["j_end"].value_or(0);
      Delimiter init{i_init, j_init}, end{i_end, j_end};
      if (type == "Desert")
        biomes.push_back(std::make_unique<Desert>(init, end));
      else if (type == "Forest")
        biomes.push_back(std::make_unique<Forest>(init, end));
      else if (type == "Dungeon")
        biomes.push_back(std::make_unique<Dungeon>(init, end));
    }
  }

  if (auto *citArr = tbl["cities"].as_array()) {
    for (auto &elem : *citArr) {
      auto &c = *elem.as_table();
      int i_init = c["i_init"].value_or(0);
      int j_init = c["j_init"].value_or(0);
      int i_end = c["i_end"].value_or(0);
      int j_end = c["j_end"].value_or(0);
      cities.emplace_back(Delimiter{i_init, j_init}, Delimiter{i_end, j_end});
    }
  }
}

int MapLoader::GetMaxSize() const { return maxSize; }

int MapLoader::GetGridSize() const { return gridSize; }

int MapLoader::GetCommonGroundTextureId() const {
  return commonGroundTextureId;
}

const std::set<std::tuple<int, int, int>> &
MapLoader::GetCollidableCells() const {
  return collidableCells;
}

const std::list<TileOrigin> &MapLoader::GetTextureOrigins() const {
  return textureOrigins;
}

std::list<std::unique_ptr<Biome>> &MapLoader::GetBiomes() { return biomes; }

std::list<City> &MapLoader::GetCities() { return cities; }
