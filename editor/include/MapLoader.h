#ifndef EDITOR_MAP_LOADER_H
#define EDITOR_MAP_LOADER_H

#include <list>
#include <map>
#include <set>
#include <string>
#include "BiomeDataParser.h"
#include <utility>

struct TileOrigin;
struct BiomeGrid;

class MapLoader {

private:

  BiomeDataParser parser;

public:
  bool loadMap(const std::string &path, std::list<TileOrigin> &txtOrigins,
               std::set<std::pair<int, int>> &collidableCells,
               std::map<int, BiomeGrid> &biomes);
};

#endif
