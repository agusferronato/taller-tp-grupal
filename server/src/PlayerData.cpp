#include "PlayerData.h"

void PlayerData::setName(const std::string &s) {
  std::strncpy(name, s.c_str(), PLAYER_NAME_MAX - 1);
  name[PLAYER_NAME_MAX - 1] = '\0';
}

void PlayerData::setRace(const std::string &s) {
  std::strncpy(race, s.c_str(), PLAYER_RACE_MAX - 1);
  race[PLAYER_RACE_MAX - 1] = '\0';
}

void PlayerData::setPlayerClass(const std::string &s) {
  std::strncpy(playerClass, s.c_str(), PLAYER_CLASS_MAX - 1);
  playerClass[PLAYER_CLASS_MAX - 1] = '\0';
}
