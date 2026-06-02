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
  Player(std::string name, Race race, Direction direction,
         PlayerClass playerClass, int x, int y, PlayerStatsInfo &statsInfo);
  void startMoving(Direction dir);
  void stopMoving();
  void move(int newX, int newY);
};

#endif
