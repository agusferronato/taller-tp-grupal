#ifndef PLAYER_STATS_H
#define PLAYER_STATS_H

#include "PlayerClass.h"
#include "Race.h"
#include <cstdint>

struct PlayerStatsInfo {
  uint32_t health;
  uint32_t mana;
  uint32_t level;
  uint32_t experience;
  uint32_t gold;
  Race race;
  PlayerClass playerClass;
};

class PlayerStats {
private:
  uint32_t gold;
  uint32_t experience;
  uint32_t level;
  uint32_t health;
  uint32_t maxHealth;
  uint32_t mana;
  uint32_t maxMana;
  uint32_t strength;
  uint32_t agility;
  uint32_t constitution;
  uint32_t intelligence;

public:
  explicit PlayerStats(const PlayerStatsInfo &info);
  PlayerStats(Race race, PlayerClass playerClass);

private:
  void initStats(Race race, PlayerClass playerClass);

public:
  void updateStats(uint32_t hp, uint32_t maxHp, uint32_t mana, uint32_t maxMana,
                   uint32_t gold, uint32_t level, uint32_t experience);

  uint32_t getGold() const { return gold; }
  uint32_t getExperience() const { return experience; }
  uint32_t getLevel() const { return level; }
  uint32_t getHp() const { return health; }
  uint32_t getMaxHp() const { return maxHealth; }
  uint32_t getMana() const { return mana; }
  uint32_t getMaxMana() const { return maxMana; }
  uint32_t getStrength() const { return strength; }
  uint32_t getAgility() const { return agility; }
  uint32_t getConstitution() const { return constitution; }
  uint32_t getIntelligence() const { return intelligence; }
};

#endif
