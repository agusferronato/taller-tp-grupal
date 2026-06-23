#ifndef INFO_H
#define INFO_H

#include <string>

#define GRASS_TEXTURE_ID 0

struct TextureData {
  std::string key;
  int id;
  std::string name;
  std::string path;
  bool collidable;
  bool transparent;
  int priority;
  float collidablePercentage{1.0f};
};

#endif
