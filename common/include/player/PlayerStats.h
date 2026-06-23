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
  Race race;
  PlayerClass playerClass;

public:
  explicit PlayerStats(const PlayerStatsInfo &info);
  PlayerStats(Race race, PlayerClass playerClass);

private:
  void initStats(Race race, PlayerClass playerClass);

public:
  void updateStats(uint32_t hp, uint32_t maxHp, uint32_t mana, uint32_t maxMana,
                   uint32_t gold, uint32_t level, uint32_t experience);

  uint32_t getGold() const;
  uint32_t getExperience() const;
  uint32_t getLevel() const;
  uint32_t getHp() const;
  uint32_t getMaxHp() const;
  uint32_t getMana() const;
  uint32_t getMaxMana() const;
  void addMana(uint32_t amount);
  uint32_t getStrength() const;
  uint32_t getAgility() const;
  uint32_t getConstitution() const;
  uint32_t getIntelligence() const;

  void setHealth(uint32_t hp);
  void setMana(uint32_t mana);

  void takeDamage(uint32_t damage);
  bool addExperience(uint32_t xp);
  void setLevel(uint32_t level);
  void setGold(uint32_t amount);

  void earnGold(uint32_t amount);
  void die();
  void heal(uint32_t amount);
};

#endif
