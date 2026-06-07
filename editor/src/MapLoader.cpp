#include "MapLoader.h"
#include "Grid.h"
#include <toml++/toml.hpp>

bool MapLoader::loadMap(const std::string &path,
                        std::list<TileOrigin> &txtOrigins,
                        std::set<std::pair<int, int>> &collidableCells,
                        std::map<int, BiomeGrid> &biomes) {
  try {
    auto tbl = toml::parse_file(path);

    if (auto *textures = tbl.as_table()->get("textures")) {
      if (auto *arr = textures->as_array()) {
        for (auto &tex : *arr) {
          auto t = tex.as_table();
          TileOrigin origin;
          origin.texture_id = t->at("txt_id").as_integer()->get();
          origin.x = t->at("i").as_integer()->get();
          origin.y = t->at("j").as_integer()->get();
          origin.priority = t->at("priority").as_integer()->get();
          txtOrigins.push_back(origin);
        }
      }
    }

    if (auto *cellsArr = tbl.as_table()->get("collidable_cells")) {
      if (auto *arr = cellsArr->as_array()) {
        for (auto &cell : *arr) {
          auto c = cell.as_table();
          int i = c->at("i").as_integer()->get();
          int j = c->at("j").as_integer()->get();
          collidableCells.insert({i, j});
        }
      }
    }

    int biomeId = 0;
    auto loadGroup = [&](const std::string &key) {
      if (auto *group = tbl.as_table()->get(key)) {
        if (auto *arr = group->as_array()) {
          for (auto &b : *arr) {
            auto bt = b.as_table();
            BiomeGrid bg{};
            bg.id = biomeId;
            bg.asString = bt->at("type").as_string()->get();
            bg.color = parser.getBiomeColor(parser.getBiomeByStr(bg.asString));
            bg.i_init = bt->at("i_init").as_integer()->get();
            bg.j_init = bt->at("j_init").as_integer()->get();
            bg.i_end = bt->at("i_end").as_integer()->get();
            bg.j_end = bt->at("j_end").as_integer()->get();
            bg.initialized = true;
            biomes[biomeId++] = bg;
          }
        }
      }
    };
    loadGroup("biomes");
    loadGroup("cities");

    return true;
  } catch (...) {
    return false;
  }
}
