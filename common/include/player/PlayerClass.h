#ifndef PLAYER_CLASS_H
#define PLAYER_CLASS_H

#include <cstdint>
#include <stdexcept>
#include <string>

enum class PlayerClass : uint8_t { Mage, Priest, Paladin, Warrior };

class PlayerClassUtils {
public:
  static PlayerClass stringToPlayerClass(const std::string &playerClassStr);
  static std::string playerClassToString(const PlayerClass playerClass);
};
#endif
