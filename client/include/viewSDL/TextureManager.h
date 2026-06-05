#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <SDL2pp/SDL2pp.hh>
#include <toml++/toml.hpp>

#include "Direction.h"
#include "TextureLayout.h"
#include "TextureLayoutType.h"

struct Sprite {
  SDL2pp::Texture &txt;
  int x, y, w, h;
};

class TextureManager {
private:
  SDL2pp::Renderer &renderer;
  std::map<int, SDL2pp::Texture> textures;
  std::map<TextureLayoutType, TextureLayout> texturesFrames;

  std::map<Direction, std::vector<SpriteData>>
  parseBodyFrames(const toml::table &table);
  std::map<Direction, SpriteData> parseHeadFrames(const toml::table &table);

public:
  explicit TextureManager(SDL2pp::Renderer &renderer);

  void loadTexturesFromToml(const std::string &path);
  void loadLayoutsFromToml(const std::string &path);

  Sprite getBodySprite(uint32_t bodyId, Direction dir, unsigned int it);
  Sprite getBodySprite(TextureLayoutType layoutType, int bodyId, Direction dir,
                       unsigned int it);
  Sprite getHeadSprite(int headId, Direction dir);
  Sprite getZombieSprite(int txtID, Direction dir, unsigned int it);
};

#endif
