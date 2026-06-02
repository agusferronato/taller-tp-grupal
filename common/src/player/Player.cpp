#include "Player.h"

Player::Player(std::string name, Race race, Direction direction,
               PlayerClass playerClass, int x, int y,
               PlayerStatsInfo &statsInfo)
    : name(std::move(name)), race(race), direction(direction),
      playerClass(playerClass), x(x), y(y), moving(false), stats(statsInfo) {}

Player::Player(std::string name, Race race, Direction direction,
               PlayerClass playerClass, int x, int y)
    : name(std::move(name)), race(race), direction(direction),
      playerClass(playerClass), x(x), y(y), moving(false),
      stats(race, playerClass) {}
