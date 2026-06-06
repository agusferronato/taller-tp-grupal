#include "Player.h"
#include "Formulas.h"

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

void Player::updateStats(uint32_t hp, uint32_t maxHp, uint32_t mana,
                         uint32_t maxMana, uint32_t gold, uint32_t level,
                         uint32_t experience) {
  stats.updateStats(hp, maxHp, mana, maxMana, gold, level, experience);
}

void Player::startMoving(Direction dir) {
  direction = dir;
  moving = true;
}

void Player::stopMoving() { moving = false; }

void Player::move(int newX, int newY) {
  x = newX;
  y = newY;
}

uint32_t Player::takeDamage(uint32_t damage) {
  uint32_t defence =
      Formulas::calcularDefensa(0, 0, 0, 0, 0, 0, rand(), rand(), rand());
  uint32_t actualDamage = damage > defence ? damage - defence : 0;
  stats.takeDamage(actualDamage);
  return actualDamage;
}

uint32_t Player::heal(uint32_t) { return 0; }
bool Player::useMana(uint32_t) { return false; }
void Player::addMana(uint32_t amount) { stats.addMana(amount); }
bool Player::gainExperience(uint32_t xp) { return stats.addExperience(xp); }
bool Player::expentGold(uint32_t amount) {
  if (stats.getGold() < amount)
    return false;
  stats.setGold(stats.getGold() - amount);
  return true;
}
void Player::earnGold(uint32_t amount) {
  stats.setGold(stats.getGold() + amount);
}
void Player::removeGold(uint32_t amount) {
  if (amount >= stats.getGold()) {
    stats.setGold(0);
  } else {
    stats.setGold(stats.getGold() - amount);
  }
}

uint32_t Player::atack() const {
  return Formulas::calcularDaño(stats.getStrength(), 0, 0, rand());
}
