#include "EquipParser.h"
#include <stdexcept>

EquipInfo EquipParser::getInfo(uint8_t itemId) const {
  switch (itemId) {
  case 1:  return {"EquipableGrid", 200, -2, -3};//
  case 2:  return {"EquipableGrid", 201, -2, -6};//
  case 3:  return {"EquipableGrid", 202, -2, -5};//
  case 4:  return {"EquipableGrid", 203, -1, -5};//
  case 5:  return {"EquipableGrid", 204, -1, -5};//
  case 6:  return {"EquipableGrid", 208, -3, -8};//
  case 7:  return {"EquipableGrid", 206, -4, -2};//
  case 8:  return {"EquipableGrid", 207, -3, -7};//
  case 9:  return {"EquipableGrid", 205, -3, -7};//
  case 10: return {"Body", 215};
  case 11: return {"Body", 216};
  case 12: return {"Tunic", 209};
  case 13: return {"Helmet", 210,0, -15, 20,60};//
  case 14: return {"Helmet", 211,0, -15, 20,60};
  case 15: return {"Helmet", 212,0, -30, 19, 60};//
  case 16: return {"EquipableGrid", 214,-2,-2,25};//
  case 17: return {"EquipableGrid", 213,-2,-2,25};//
  default:
    throw std::runtime_error("Unknown equip item ID: " + std::to_string(itemId));
  }
}
