#ifndef EQUIP_PARSER_H
#define EQUIP_PARSER_H

#include <cstdint>
#include <string>

struct EquipInfo {
  std::string type;
  int textureId;
  int offsetX = 0;
  int offsetY = 0;
  int renderWidth = -1;
  int renderHeight = -1;
};

class EquipParser {
public:
  EquipInfo getInfo(uint8_t itemId) const;
};

#endif
