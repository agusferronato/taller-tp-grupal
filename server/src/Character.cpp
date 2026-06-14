#include "Character.h"
#include "City.h"
#include "Formulas.h"
#include "PlayerClass.h"
#include "Race.h"
#include <algorithm>

Character::Character(uint32_t id, std::string name, Race race,
                     PlayerClass playerClass, int x, int y, Direction dir)
    : id(id), player(std::move(name), race, dir, playerClass, x, y) {}

Character::Character(uint32_t id, const PlayerData &data)
    : id(id), player(data.name, RaceUtils::stringToRace(data.race),
                     static_cast<Direction>(data.direction),
                     PlayerClassUtils::stringToPlayerClass(data.playerClass),
                     data.x, data.y) {
  player.updateStats(data.hp, data.maxHp, data.mana, data.maxMana, data.gold,
                     data.level, data.experience);
  player.setInventoryItems(data.inventory);
  player.setEquippedWeapon(data.equippedWeapon);
  player.setEquippedArmor(data.equippedArmor);
  player.setEquippedHelmet(data.equippedHelmet);
  player.setEquippedShield(data.equippedShield);
  player.joinClan(data.clanId);
}

PlayerData Character::toPlayerData() const {
  PlayerData data{};
  data.setName(player.getName());
  data.setRace(RaceUtils::raceToString(player.getRace()));
  data.setPlayerClass(
      PlayerClassUtils::playerClassToString(player.getPlayerClass()));
  data.x = player.getX();
  data.y = player.getY();
  data.direction = static_cast<uint8_t>(player.getDirection());
  data.level = player.getLevel();
  data.hp = player.getHp();
  data.maxHp = player.getMaxHp();
  data.mana = player.getMana();
  data.maxMana = player.getMaxMana();
  data.experience = player.getExperience();
  data.gold = player.getGold();
  data.strength = player.getStrength();
  data.agility = player.getAgility();
  data.constitution = player.getConstitution();
  data.intelligence = player.getIntelligence();
  data.inventory = player.getInventoryItems();
  data.equippedWeapon = player.getEquippedWeapon();
  data.equippedArmor = player.getEquippedArmor();
  data.equippedHelmet = player.getEquippedHelmet();
  data.equippedShield = player.getEquippedShield();
  data.clanId = player.getClanId();
  return data;
}

PlayerInfoDTO Character::toPlayerInfo(uint32_t playerId) const {
  return {playerId,
          static_cast<int16_t>(player.getX()),
          static_cast<int16_t>(player.getY()),
          player.getDirection(),
          player.getRace(),
          player.getPlayerClass(),
          player.getName(),
          player.getHp(),
          player.getMaxHp(),
          player.getMana(),
          player.getMaxMana(),
          player.getGold(),
          player.getLevel(),
          player.getExperience()};
}

void Character::setDirection(Direction dir) { player.startMoving(dir); }

void Character::stop() { player.stopMoving(); }

std::pair<int, int> Character::getTargetPosition(Direction dir) const {
  return getTargetPosition(dir, 2);
}

std::pair<int, int> Character::getTargetPosition(Direction dir,
                                                 uint32_t speed) const {
  int targetX = player.getX();
  int targetY = player.getY();
  switch (dir) {
  case Direction::Up:
    targetY -= speed;
    break;
  case Direction::Down:
    targetY += speed;
    break;
  case Direction::Left:
    targetX -= speed;
    break;
  case Direction::Right:
    targetX += speed;
    break;
  }
  return {targetX, targetY};
}

uint32_t Character::takeDamage(uint32_t damage) {
  return player.takeDamage(damage);
}

void Character::heal(uint32_t amount) { player.heal(amount); }

void Character::gainExperience(uint32_t xp) { player.gainExperience(xp); }

void Character::addGold(uint32_t amount) { player.earnGold(amount); }

void Character::setGold(uint32_t amount) { player.setGold(amount); }

void Character::spendGold(uint32_t amount) { player.expentGold(amount); }

bool Character::colisionaCon(int targetX, int targetY, int ancho,
                             int alto) const {
  return !(targetX + ancho <= player.getX() ||
           targetX >= player.getX() + ANCHO ||
           targetY + alto <= player.getY() || targetY >= player.getY() + ALTO);
}

bool Character::isInCity(std::list<City> &cities, int gridSize, int maxSize) {
  for (auto &city : cities) {
    if (city.contains(player.getX(), player.getY(), gridSize, maxSize)) {
      return true;
    }
  }
  return false;
}

int Character::getX() const { return player.getX(); }

int Character::getY() const { return player.getY(); }

int Character::getAncho() const { return ANCHO; }

int Character::getAlto() const { return ALTO; }

PlayerInfoEventDTO Character::toPlayerInfoEvent() const {
  return {id,
          player.getHp(),
          player.getMaxHp(),
          player.getMana(),
          player.getMaxMana(),
          player.getGold(),
          player.getLevel(),
          player.getExperience()};
}

PlayerAppearedEventDTO Character::toPlayerAppeared() const {
  return {id,
          static_cast<int16_t>(player.getX()),
          static_cast<int16_t>(player.getY()),
          player.getDirection(),
          player.getRace(),
          player.getPlayerClass(),
          player.getName(),
          player.getHp(),
          player.getMaxHp(),
          player.getMana(),
          player.getMaxMana(),
          player.getGold(),
          player.getLevel(),
          player.getExperience()};
}

PlayerMovedEventDTO Character::toPlayerMoved() const {
  return {id, static_cast<int16_t>(player.getX()),
          static_cast<int16_t>(player.getY()), player.getDirection()};
}

std::pair<int, int> Character::getTargetPosition() const {
  return getTargetPosition(player.getDirection());
}

std::pair<int, int> Character::getTargetPosition(uint32_t speed) const {
  return getTargetPosition(player.getDirection(), speed);
}

uint32_t Character::getDamage() const { return player.attack(); }

bool Character::tryParry() const {
  return Formulas::calcularEsquivo(player.getAgility(), rand() % 2);
}

bool Character::assertAttackDistance(int16_t targetX, int16_t targetY) const {
  return player.assertAttackDistance(targetX, targetY);
}

uint32_t Character::dropGoldOnDeath() {
  uint32_t perdido =
      Formulas::calcularOroPerdidoMuerte(player.getGold(), player.getLevel());
  player.removeGold(perdido);
  return perdido;
}

std::vector<uint8_t> Character::die() { return player.die(); }

bool Character::hasItem(uint8_t itemId) const {
  return player.getInventory().findItem(itemId) != MAX_INVENTORY_SLOTS;
}

bool Character::hasMoney(uint16_t amount) const {
  return player.getGold() >= amount;
}


void Character::removeItemById(uint8_t itemId) {
  uint8_t slot = player.getInventory().findItem(itemId);
  if (slot != MAX_INVENTORY_SLOTS)
    player.removeItem(slot);
}

void Character::resurrect() {
  player.resurrect();
}
