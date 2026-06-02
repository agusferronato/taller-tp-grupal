#include "Player.h"

Player::Player(std::string name, Direction direction, int x, int y,
               const PlayerStatsInfo &statsInfo)
    : name(std::move(name)), race(statsInfo.race), direction(direction),
      playerClass(statsInfo.playerClass), x(x), y(y), moving(false),
      stats(statsInfo) {}

Player::Player(std::string name, Race race, Direction direction,
               PlayerClass playerClass, int x, int y)
    : name(std::move(name)), race(race), direction(direction),
      playerClass(playerClass), x(x), y(y), moving(false),
      stats(race, playerClass) {}

void Player::startMoving(Direction dir) {
  direction = dir;
  moving = true;
}

void Player::stopMoving() { moving = false; }

void Player::move(int newX, int newY) {
  x = newX;
  y = newY;
}

void Player::takeDamage(uint32_t) {}
uint32_t Player::heal(uint32_t) { return 0; }
bool Player::useMana(uint32_t) { return false; }
bool Player::gainExperience(uint32_t) { return false; }
bool Player::expentGold(uint32_t) { return false; }
void Player::earnGold(uint32_t) {}
uint32_t Player::atack() { return 0; }
