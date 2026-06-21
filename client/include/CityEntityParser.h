#ifndef CITY_ENTITY_PARSER_H
#define CITY_ENTITY_PARSER_H

#include "CityEntityType.h"
#include "TextureLayoutType.h"
#include <string>

struct CityEntityInfo {
  int textureId;
  TextureLayoutType layoutType;
  std::string name;
};

class CityEntityParser {
public:
  CityEntityInfo getInfo(CityEntityType type) const;
};

#endif
