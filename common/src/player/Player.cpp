#include "Player.h"
#include "Formulas.h"
#include "ItemData.h"

bool Player::isMoving() const { return moving; }
Direction Player::getDirection() const { return direction; }
int Player::getX() const { return x; }
int Player::getY() const { return y; }
Race Player::getRace() const { return race; }
const std::string &Player::getName() const { return name; }
uint32_t Player::getHp() const { return stats.getHp(); }
uint32_t Player::getMaxHp() const { return stats.getMaxHp(); }
uint32_t Player::getMana() const { return stats.getMana(); }
uint32_t Player::getMaxMana() const { return stats.getMaxMana(); }
uint32_t Player::getGold() const { return stats.getGold(); }
uint32_t Player::getLevel() const { return stats.getLevel(); }
uint32_t Player::getExperience() const { return stats.getExperience(); }
PlayerClass Player::getPlayerClass() const { return playerClass; }
uint32_t Player::getStrength() const { return stats.getStrength(); }
uint32_t Player::getAgility() const { return stats.getAgility(); }
uint32_t Player::getConstitution() const { return stats.getConstitution(); }
uint32_t Player::getIntelligence() const { return stats.getIntelligence(); }
Inventory &Player::getInventory() { return inventory; }
const Inventory &Player::getInventory() const { return inventory; }
bool Player::addItem(uint8_t itemId) { return inventory.addItem(itemId); }
bool Player::equipItem(uint8_t slotIndex) {
  return inventory.equipItem(slotIndex);
}
bool Player::unequipSlot(EquipSlot slot) { return inventory.unequipSlot(slot); }
bool Player::removeItem(uint8_t slotIndex) {
  return inventory.removeItem(slotIndex);
}
std::array<uint8_t, MAX_INVENTORY_SLOTS> Player::getInventoryItems() const {
  return inventory.getItems();
}
const Weapon &Player::getEquippedWeapon() const {
  return inventory.getWeapon();
}
const Armor &Player::getEquippedArmor() const { return inventory.getArmor(); }
const Helmet &Player::getEquippedHelmet() const {
  return inventory.getHelmet();
}
const Shield &Player::getEquippedShield() const {
  return inventory.getShield();
}
void Player::setInventoryItems(
    const std::array<uint8_t, MAX_INVENTORY_SLOTS> &items) {
  inventory.setItems(items);
}
void Player::setEquippedWeapon(uint8_t id) { inventory.setWeapon(id); }
void Player::setEquippedArmor(uint8_t id) { inventory.setArmor(id); }
void Player::setEquippedHelmet(uint8_t id) { inventory.setHelmet(id); }
void Player::setEquippedShield(uint8_t id) { inventory.setShield(id); }
uint32_t Player::getClanId() const { return clanId; }
bool Player::hasClan() const { return clanId != NO_CLAN; }
void Player::joinClan(uint32_t clanId) { this->clanId = clanId; }
void Player::leaveClan() { this->clanId = NO_CLAN; }
bool Player::isDead() const { return death; }

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
  if (hp <= 0)
    death = true;
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

uint32_t Player::takeDamage(uint32_t damage, int defenseBonusPercent) {
  Armor armor = getEquippedArmor();
  Shield shield = getEquippedShield();
  Helmet helmet = getEquippedHelmet();
  uint32_t defence = Formulas::calcularDefensa(
      armor.minDefense(), armor.maxDefense(), shield.minDefense(),
      shield.maxDefense(), helmet.minDefense(), helmet.maxDefense(), rand(),
      rand(), rand());
  if (defenseBonusPercent > 0) {
    defence += defence * static_cast<uint32_t>(defenseBonusPercent) / 100;
  }
  uint32_t actualDamage = damage > defence ? damage - defence : 0;
  stats.takeDamage(actualDamage);
  return actualDamage;
}

void Player::heal(uint32_t amount) { stats.heal(amount); }

void Player::resurrect() {
  death = false;
  stats.setHealth(stats.getMaxHp());
  stats.setMana(stats.getMaxMana());
}
bool Player::useMana(uint32_t amount) {
  if (stats.getMana() < amount)
    return false;
  stats.setMana(stats.getMana() - amount);
  return true;
}
void Player::addMana(uint32_t amount) { stats.addMana(amount); }
bool Player::gainExperience(uint32_t xp) { return stats.addExperience(xp); }
bool Player::expentGold(uint32_t amount) {
  if (stats.getGold() < amount)
    return false;
  stats.setGold(stats.getGold() - amount);
  return true;
}
void Player::earnGold(uint32_t amount) { stats.earnGold(amount); }

void Player::setGold(uint32_t amount) { stats.setGold(amount); }

void Player::removeGold(uint32_t amount) {
  if (amount >= stats.getGold()) {
    stats.setGold(0);
  } else {
    stats.setGold(stats.getGold() - amount);
  }
}

uint32_t Player::attack() const {
  Weapon weapon = getEquippedWeapon();
  return Formulas::calcularDaño(stats.getStrength(), weapon.minDamage(),
                                weapon.maxDamage(),
                                static_cast<double>(std::rand()) / RAND_MAX);
}

void Player::setLevel(uint32_t level) {
  stats.setLevel(level);
  death = false;
}

bool Player::assertAttackDistance(int16_t targetX, int16_t targetY) const {
  int16_t dx = std::abs(targetX - x);
  int16_t dy = std::abs(targetY - y);
  if (dx <= 64 && dy <= 64) {
    return true;
  }
  Weapon weapon = getEquippedWeapon();
  return weapon.longDistance() && weapon.hasRange(dx, dy);
}

std::vector<uint8_t> Player::die() {
  death = true;
  stats.die();
  std::vector<uint8_t> droppedItems;
  auto ids = inventory.getItems();
  for (int i = 0; i < MAX_INVENTORY_SLOTS; i++) {
    if (ids[i] != 0) {
      inventory.removeItem(i);
      droppedItems.push_back(ids[i]);
    }
  }
  uint8_t weaponId = inventory.getWeapon().getID();
  if (weaponId != 0) {
    inventory.setWeapon(0);
    droppedItems.push_back(weaponId);
  }
  uint8_t armorId = inventory.getArmor().getID();
  if (armorId != 0) {
    inventory.setArmor(0);
    droppedItems.push_back(armorId);
  }
  uint8_t helmetId = inventory.getHelmet().getID();
  if (helmetId != 0) {
    inventory.setHelmet(0);
    droppedItems.push_back(helmetId);
  }
  uint8_t shieldId = inventory.getShield().getID();
  if (shieldId != 0) {
    inventory.setShield(0);
    droppedItems.push_back(shieldId);
  }
  return droppedItems;
}
