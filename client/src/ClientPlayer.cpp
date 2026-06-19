#include "ClientPlayer.h"

void ClientPlayer::setInventory(const std::array<uint8_t, MAX_INVENTORY_SLOTS> &items) { player.setInventoryItems(items); }
void ClientPlayer::setEquippedWeapon(uint8_t id) { player.setEquippedWeapon(id); }
void ClientPlayer::setEquippedArmor(uint8_t id) { player.setEquippedArmor(id); }
void ClientPlayer::setEquippedHelmet(uint8_t id) { player.setEquippedHelmet(id); }
void ClientPlayer::setEquippedShield(uint8_t id) { player.setEquippedShield(id); }
bool ClientPlayer::getIsMoving() const { return player.isMoving(); }
Direction ClientPlayer::getDirection() const { return player.getDirection(); }
int ClientPlayer::get_x() const { return player.getX(); }
int ClientPlayer::get_y() const { return player.getY(); }
Race ClientPlayer::getRace() const { return player.getRace(); }
const std::string &ClientPlayer::getName() const { return player.getName(); }
uint32_t ClientPlayer::getHp() const { return player.getHp(); }
uint32_t ClientPlayer::getMaxHp() const { return player.getMaxHp(); }
uint32_t ClientPlayer::getMana() const { return player.getMana(); }
uint32_t ClientPlayer::getMaxMana() const { return player.getMaxMana(); }
uint32_t ClientPlayer::getGold() const { return player.getGold(); }
uint32_t ClientPlayer::getLevel() const { return player.getLevel(); }
uint32_t ClientPlayer::getExperience() const { return player.getExperience(); }
const Player &ClientPlayer::getPlayer() const { return player; }
bool ClientPlayer::isBeingAttackOrCured() const { return beingAttacked; }
EffectType ClientPlayer::getEffect() const { return currentEffect; }
void ClientPlayer::setBeingAttacked(bool v, EffectType effect) { beingAttacked = v; if (v) currentEffect = effect; }
void ClientPlayer::stopAttackEffect() const { beingAttacked = false; currentEffect = EffectType::NormalAttack; }
uint8_t ClientPlayer::getEquippedWeapon() const { return player.getEquippedWeapon().getID(); }
uint8_t ClientPlayer::getEquippedArmor() const { return player.getEquippedArmor().getID(); }
uint8_t ClientPlayer::getEquippedHelmet() const { return player.getEquippedHelmet().getID(); }
uint8_t ClientPlayer::getEquippedShield() const { return player.getEquippedShield().getID(); }
const Inventory &ClientPlayer::getInventory() const { return player.getInventory(); }

ClientPlayer::ClientPlayer(uint32_t id, std::string name, int xOrigin,
                           int yOrigin, Direction direction,
                           const PlayerStatsInfo &statsInfo)
    : id(id), player(name, direction, xOrigin, yOrigin, statsInfo) {}

uint32_t ClientPlayer::getID() const { return id; }

void ClientPlayer::updateCoordinates(int x, int y, Direction direction) {
  player.move(x, y);
  player.startMoving(direction);
}

void ClientPlayer::stopMoving() { player.stopMoving(); }

void ClientPlayer::updateStats(uint32_t hp, uint32_t maxHp, uint32_t mana,
                               uint32_t maxMana, uint32_t gold, uint32_t level,
                               uint32_t experience) {
  player.updateStats(hp, maxHp, mana, maxMana, gold, level, experience);
}

void ClientPlayer::die() { player.die(); }

void ClientPlayer::resurrect(int x, int y) {
  player.move(x, y);
  player.resurrect();
}

bool ClientPlayer::isDead() const { return player.isDead(); }
