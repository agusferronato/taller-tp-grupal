#ifndef TEXTURE_MAPPER_H
#define TEXTURE_MAPPER_H

#include <SDL2pp/SDL2pp.hh>
#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "MapData.h"

class TextureMapper {
private:
  struct TextureInfo {
    SDL2pp::Texture texture;
    int width;
    int height;
  };

  SDL2pp::Renderer &renderer;
  std::map<int, TextureInfo> textures;
  std::vector<std::map<std::pair<int, int>, std::vector<GridItem>>>
      tilesToRender;

public:
  explicit TextureMapper(SDL2pp::Renderer &renderer);

  void loadFromToml(const std::string &path);
  void buildRenderGrid(const std::list<TileOrigin> &origins, int gridSizePx);

  SDL2pp::Texture &getTexture(int id);
  const std::vector<std::map<std::pair<int, int>, std::vector<GridItem>>> &
  getTilesToRender() const;
};

#endif
