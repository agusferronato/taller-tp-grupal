#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include <cstdint>
#include <cstring>
#include <string>

#define PLAYER_NAME_MAX 32
#define PLAYER_PASS_MAX 32
#define PLAYER_RACE_MAX 16
#define PLAYER_CLASS_MAX 16

struct PlayerData {
  char name[PLAYER_NAME_MAX];
  char password[PLAYER_PASS_MAX];
  char race[PLAYER_RACE_MAX];
  char playerClass[PLAYER_CLASS_MAX];
  int32_t x, y;
  uint8_t direction;
  uint32_t level;
  uint32_t hp, maxHp;
  uint32_t mana, maxMana;
  uint32_t experience;
  uint32_t gold;

  void setName(const std::string &s) { std::strncpy(name, s.c_str(), PLAYER_NAME_MAX - 1); name[PLAYER_NAME_MAX - 1] = '\0'; }
  void setPassword(const std::string &s) { std::strncpy(password, s.c_str(), PLAYER_PASS_MAX - 1); password[PLAYER_PASS_MAX - 1] = '\0'; }
  void setRace(const std::string &s) { std::strncpy(race, s.c_str(), PLAYER_RACE_MAX - 1); race[PLAYER_RACE_MAX - 1] = '\0'; }
  void setPlayerClass(const std::string &s) { std::strncpy(playerClass, s.c_str(), PLAYER_CLASS_MAX - 1); playerClass[PLAYER_CLASS_MAX - 1] = '\0'; }
};

#endif
