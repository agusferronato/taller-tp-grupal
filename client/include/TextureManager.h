#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <SDL2pp/SDL2pp.hh>

#include "Direction.h"
#include "Race.h"
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

  int getRaceBodyID(Race race) const;
  int getRaceHeadID(Race race) const;

public:
  explicit TextureManager(SDL2pp::Renderer &renderer);

  void loadTexturesFromToml(const std::string &path);
  void loadLayoutsFromToml(const std::string &path);

  Sprite getBodySprite(Race race, Direction dir, unsigned int it);
  Sprite getHeadSprite(Race race, Direction dir);
};

#endif
