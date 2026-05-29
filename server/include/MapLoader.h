#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <list>
#include <set>
#include <string>
#include <tuple>

#include "MapData.h"

class MapLoader {
private:
  int maxSize;
  int gridSize;
  int commonGroundTextureId;
  std::set<std::tuple<int, int, int>> collidableCells;
  std::list<TileOrigin> textureOrigins;

public:
  explicit MapLoader(const std::string &path);

  int GetMaxSize() const;
  int GetGridSize() const;
  int GetCommonGroundTextureId() const;
  const std::set<std::tuple<int, int, int>> &GetCollidableCells() const;
  const std::list<TileOrigin> &GetTextureOrigins() const;
};

#endif
