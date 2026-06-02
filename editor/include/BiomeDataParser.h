#ifndef BIOME_DATA_PARSER_H
#define BIOME_DATA_PARSER_H

#include "Biome.h"
#include <SDL2pp/SDL2pp.hh>
#include <string>

class BiomeDataParser {

public:
  std::string getBiomeAsString(Biome biome);
  SDL2pp::Color getBiomeColor(Biome biome);
};

#endif
