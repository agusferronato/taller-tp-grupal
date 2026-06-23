#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <list>
#include <memory>
#include <set>
#include <string>
#include <tuple>

#include "Biome.h"
#include "City.h"
#include "MapData.h"

class MapLoader {
private:
  int maxSize;
  int gridSize;
  int commonGroundTextureId;
  std::set<std::tuple<int, int, int>> collidableCells;
  std::list<TileOrigin> textureOrigins;
  std::list<std::unique_ptr<Biome>> biomes;
  std::list<City> cities;

public:
  explicit MapLoader(const std::string &path);

  int GetMaxSize() const;
  int GetGridSize() const;
  int GetCommonGroundTextureId() const;
  const std::set<std::tuple<int, int, int>> &GetCollidableCells() const;
  const std::list<TileOrigin> &GetTextureOrigins() const;
  std::list<std::unique_ptr<Biome>> &GetBiomes();
  std::list<City> &GetCities();
};

#endif
