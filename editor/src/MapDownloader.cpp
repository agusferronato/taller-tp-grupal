#include "MapDownloader.h"
#include "Grid.h"
#include "GridSDL.h"
#include "Info.h"
#include <fstream>
#include <toml++/toml.hpp>

MapDownloader::MapDownloader(const std::string &path) : path(path) {}

void MapDownloader::saveMap(
    GridSDL &gridSDL, std::list<TileOrigin> &txtOrigins,
    std::set<std::tuple<int, int, int>> &collidableCells,
    const std::vector<BiomeGrid> &biomes) {

  toml::table tbl;

  tbl.emplace("MAX_SIZE", MAX_SIZE);
  tbl.emplace("GRID_SIZE", GRID_SIZE_PX);
  tbl.emplace("COMMON_GROUND_TEXTURE_ID", GRASS_TEXTURE_ID);

  toml::array textures_arr;
  for (auto &origin : txtOrigins) {
    auto i = origin.x;
    auto j = origin.y;
    toml::table tex;
    tex.emplace("txt_id", origin.texture_id);
    tex.emplace("i", i);
    tex.emplace("j", j);
    tex.emplace("priority", origin.priority);
    textures_arr.push_back(std::move(tex));
  }
  tbl.emplace("textures", std::move(textures_arr));

  toml::array coll_arr;
  for (auto &cell : collidableCells) {
    auto [i, j, _] = cell;
    toml::table cell_tbl;
    cell_tbl.emplace("i", i);
    cell_tbl.emplace("j", j);
    coll_arr.push_back(std::move(cell_tbl));
  }
  tbl.emplace("collidable_cells", std::move(coll_arr));

  toml::array biomes_arr;
  toml::array cities_arr;
  for (auto &b : biomes) {
    if (!b.initialized)
      continue;
    toml::table b_tbl;
    b_tbl.emplace("type", b.asString);
    b_tbl.emplace("i_init", b.i_init);
    b_tbl.emplace("j_init", b.j_init);
    b_tbl.emplace("i_end", b.i_end);
    b_tbl.emplace("j_end", b.j_end);
    if (b.asString == "City")
      cities_arr.push_back(std::move(b_tbl));
    else
      biomes_arr.push_back(std::move(b_tbl));
  }
  tbl.emplace("biomes", std::move(biomes_arr));
  tbl.emplace("cities", std::move(cities_arr));

  std::ofstream file(path);
  if (file) {
    file << tbl << std::endl;
    emit gridSDL.mapSavedSuccesfully(path);
  }
}
