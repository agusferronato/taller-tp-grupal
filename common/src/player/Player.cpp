#include "Player.h"
#include "Formulas.h"

Player::Player(std::string name, Direction direction, int x, int y,
               const PlayerStatsInfo &statsInfo)
    : name(std::move(name)), race(statsInfo.race), direction(direction),
      playerClass(statsInfo.playerClass), x(x), y(y), moving(false),
      death(statsInfo.health == 0), stats(statsInfo) {}

Player::Player(std::string name, Race race, Direction direction,
               PlayerClass playerClass, int x, int y)
    : name(std::move(name)), race(race), direction(direction),
      playerClass(playerClass), x(x), y(y), moving(false), death(false),
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
  const auto &armor = ITEM_TABLE[getEquippedArmor()];
  const auto &shield = ITEM_TABLE[getEquippedShield()];
  const auto &helmet = ITEM_TABLE[getEquippedHelmet()];
  uint32_t defence = Formulas::calcularDefensa(
      armor.minDefense, armor.maxDefense, shield.minDefense, shield.maxDefense,
      helmet.minDefense, helmet.maxDefense, rand(), rand(), rand());
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
  stats.earnGold(stats.getGold() + amount);
}
void Player::removeGold(uint32_t amount) {
  if (amount >= stats.getGold()) {
    stats.setGold(0);
  } else {
    stats.setGold(stats.getGold() - amount);
  }
}

uint32_t Player::attack() const {
  const auto &weapon = ITEM_TABLE[getEquippedWeapon()];
  return Formulas::calcularDaño(stats.getStrength(), weapon.minDamage,
                                weapon.maxDamage, rand());
}

bool Player::assertAttackDistance(int16_t targetX, int16_t targetY) const {
  int16_t dx = std::abs(targetX - x);
  int16_t dy = std::abs(targetY - y);
  if (dx <= 64 && dy <= 64) {
    return true;
  }
  const auto &weapon = ITEM_TABLE[getEquippedWeapon()];
  return weapon.isRange;
}

std::vector<uint8_t> Player::die() {
  death = true;
  stats.die();
  std::vector<uint8_t> droppedItems;
  for (int i = 0; i < MAX_INVENTORY_SLOTS; i++) {
    uint8_t itemId = inventory.getItems()[i];
    if (itemId != EMPTY_ITEM) {
      inventory.removeItem(i);
      droppedItems.push_back(itemId);
    }
  }
  uint8_t weaponId = inventory.getWeapon();
  if (weaponId != EMPTY_ITEM) {
    inventory.setWeapon(EMPTY_ITEM);
    droppedItems.push_back(weaponId);
  }
  uint8_t armorId = inventory.getArmor();
  if (armorId != EMPTY_ITEM) {
    inventory.setArmor(EMPTY_ITEM);
    droppedItems.push_back(armorId);
  }
  uint8_t helmetId = inventory.getHelmet();
  if (helmetId != EMPTY_ITEM) {
    inventory.setHelmet(EMPTY_ITEM);
    droppedItems.push_back(helmetId);
  }
  uint8_t shieldId = inventory.getShield();
  if (shieldId != EMPTY_ITEM) {
    inventory.setShield(EMPTY_ITEM);
    droppedItems.push_back(shieldId);
  }
  return droppedItems;
}
