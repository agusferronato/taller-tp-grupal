#include "PlayerClass.h"

PlayerClass
PlayerClassUtils::stringToPlayerClass(const std::string &playerClassStr) {
  if (playerClassStr == "Mago") {
    return PlayerClass::Mage;
  } else if (playerClassStr == "Clerigo") {
    return PlayerClass::Priest;
  } else if (playerClassStr == "Paladin") {
    return PlayerClass::Paladin;
  } else if (playerClassStr == "Guerrero") {
    return PlayerClass::Warrior;
  }
  throw std::invalid_argument("Invalid PlayerClass string: " + playerClassStr);
}

std::string
PlayerClassUtils::playerClassToString(const PlayerClass playerClass) {
  switch (playerClass) {
  case PlayerClass::Mage:
    return "Mago";
  case PlayerClass::Priest:
    return "Clerigo";
  case PlayerClass::Paladin:
    return "Paladin";
  case PlayerClass::Warrior:
    return "Guerrero";
  default:
    throw std::invalid_argument("Invalid playerClass");
  }
}
