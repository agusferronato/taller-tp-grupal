#ifndef MAP_DOWNLOADER_H
#define MAP_DOWNLOADER_H

#include <list>
#include <map>
#include <set>
#include <string>
#include <tuple>

class GridSDL;

struct TileOrigin;

class MapDownloader {

private:
  const std::string path;

public:
  explicit MapDownloader(const std::string &path);

  void saveMap(GridSDL &gridSDL, std::list<TileOrigin> &txtOrigins,
               std::set<std::tuple<int, int, int>> &collidableCells);
};

#endif
