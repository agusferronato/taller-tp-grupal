#ifndef MAP_DOWNLOADER_H
#define MAP_DOWNLOADER_H

#include <list>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

class GridSDL;

struct TileOrigin;

struct BiomeGrid;

class MapDownloader {

public:
  void saveMap(GridSDL &gridSDL, const std::string &path,
               std::list<TileOrigin> &txtOrigins,
               std::set<std::pair<int, int>> &collidableCells,
               const std::map<int, BiomeGrid> &biomes);
};

#endif
