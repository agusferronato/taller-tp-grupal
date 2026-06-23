#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include <array>
#include <cstdint>
#include <cstring>
#include <string>

#include "InventoryConstants.h"

#define PLAYER_NAME_MAX 32
#define PLAYER_PASS_MAX 32
#define PLAYER_RACE_MAX 16
#define PLAYER_CLASS_MAX 16

struct PlayerData {
  char name[PLAYER_NAME_MAX];
  char race[PLAYER_RACE_MAX];
  char playerClass[PLAYER_CLASS_MAX];
  int32_t x, y;
  uint8_t direction;
  uint32_t level;
  uint32_t hp, maxHp;
  uint32_t mana, maxMana;
  uint32_t experience;
  uint32_t gold;
  uint32_t strength;
  uint32_t agility;
  uint32_t constitution;
  uint32_t intelligence;
  std::array<uint8_t, MAX_INVENTORY_SLOTS> inventory{};
  uint8_t equippedWeapon{0};
  uint8_t equippedArmor{0};
  uint8_t equippedHelmet{0};
  uint8_t equippedShield{0};
  uint32_t clanId{0};

  void setName(const std::string &s);
  void setRace(const std::string &s);
  void setPlayerClass(const std::string &s);
};

#endif
