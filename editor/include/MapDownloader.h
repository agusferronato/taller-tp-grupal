#ifndef MAP_DOWNLOADER_H
#define MAP_DOWNLOADER_H

#include <list>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <vector>

class GridSDL;

struct TileOrigin;

struct BiomeGrid;

class MapDownloader {

private:
  const std::string path;

public:
  explicit MapDownloader(const std::string &path);

  void saveMap(GridSDL &gridSDL, std::list<TileOrigin> &txtOrigins,
               std::set<std::tuple<int, int, int>> &collidableCells,
               const std::vector<BiomeGrid> &biomes);
};

#endif
