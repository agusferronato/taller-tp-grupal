#ifndef PLAYER_H
#define PLAYER_H

#include "Direction.h"
#include "Inventory.h"
#include "PlayerClass.h"
#include "PlayerStats.h"
#include "Race.h"
#include <string>
#include <utility>

class Player {
private:
  std::string name;
  Race race;
  Direction direction;
  PlayerClass playerClass;
  int x, y;
  bool moving;
  PlayerStats stats;
  Inventory inventory;

public:
  Player(std::string name, Race race, Direction direction,
         PlayerClass playerClass, int x, int y);
  Player(std::string name, Direction direction, int x, int y,
         const PlayerStatsInfo &statsInfo);

  void startMoving(Direction dir);
  void stopMoving();
  void move(int newX, int newY);
  void takeDamage(uint32_t damage);
  uint32_t heal(uint32_t amount);
  bool useMana(uint32_t amount);
  bool gainExperience(uint32_t amount);
  bool expentGold(uint32_t amount);
  void earnGold(uint32_t);
  uint32_t atack();

  bool isMoving() const { return moving; }
  Direction getDirection() const { return direction; }
  int getX() const { return x; }
  int getY() const { return y; }
  Race getRace() const { return race; }
  const std::string &getName() const { return name; }
  uint32_t getHp() const { return stats.getHp(); }
  uint32_t getMaxHp() const { return stats.getMaxHp(); }
  uint32_t getMana() const { return stats.getMana(); }
  uint32_t getMaxMana() const { return stats.getMaxMana(); }
  uint32_t getGold() const { return stats.getGold(); }
  uint32_t getLevel() const { return stats.getLevel(); }
  uint32_t getExperience() const { return stats.getExperience(); }
};

#endif
