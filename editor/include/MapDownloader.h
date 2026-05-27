#ifndef MAP_DOWNLOADER_H
#define MAP_DOWNLOADER_H


#include <string>
#include <tuple>
#include <map>
#include <list>
#include <set>

class GridSDL;

struct TileOrigin;


class MapDownloader {

private:
    const std::string path;

public:

    MapDownloader(const std::string& path);

    void saveMap(
        GridSDL& gridSDL,
        std::list<TileOrigin>& txtOrigins,
        std::set<std::tuple<int, int>>& collidableCells
    ); 

};


#endif
