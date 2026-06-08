#ifndef PLAYER_H
#define PLAYER_H

#include "Direction.h"
#include "Inventory.h"
#include "PlayerClass.h"
#include "PlayerStats.h"
#include "Race.h"
#include <string>
#include <utility>
#include <vector>

class Player {
private:
  std::string name;
  Race race;
  Direction direction;
  PlayerClass playerClass;
  int x, y;
  bool moving;
  bool death;
  PlayerStats stats;
  Inventory inventory;

public:
  Player(std::string name, Race race, Direction direction,
         PlayerClass playerClass, int x, int y);
  Player(std::string name, Direction direction, int x, int y,
         const PlayerStatsInfo &statsInfo);

  void updateStats(uint32_t hp, uint32_t maxHp, uint32_t mana, uint32_t maxMana,
                   uint32_t gold, uint32_t level, uint32_t experience);
  void startMoving(Direction dir);
  void stopMoving();
  void move(int newX, int newY);
  uint32_t takeDamage(uint32_t damage);
  uint32_t heal(uint32_t amount);
  bool useMana(uint32_t amount);
  void addMana(uint32_t amount);
  bool gainExperience(uint32_t amount);
  bool expentGold(uint32_t amount);
  void earnGold(uint32_t);
  void removeGold(uint32_t amount);
  uint32_t attack() const;

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
  PlayerClass getPlayerClass() const { return playerClass; }
  uint32_t getStrength() const { return stats.getStrength(); }
  uint32_t getAgility() const { return stats.getAgility(); }
  uint32_t getConstitution() const { return stats.getConstitution(); }
  uint32_t getIntelligence() const { return stats.getIntelligence(); }

  Inventory &getInventory() { return inventory; }
  const Inventory &getInventory() const { return inventory; }
  bool addItem(uint8_t itemId) { return inventory.addItem(itemId); }
  bool equipItem(uint8_t slotIndex) { return inventory.equipItem(slotIndex); }
  bool unequipSlot(EquipSlot slot) { return inventory.unequipSlot(slot); }
  bool removeItem(uint8_t slotIndex) { return inventory.removeItem(slotIndex); }
  const std::array<uint8_t, MAX_INVENTORY_SLOTS> &getInventoryItems() const {
    return inventory.getItems(); 
  }
  uint8_t getEquippedWeapon() const { return inventory.getWeapon(); }
  uint8_t getEquippedArmor() const { return inventory.getArmor(); }
  uint8_t getEquippedHelmet() const { return inventory.getHelmet(); }
  uint8_t getEquippedShield() const { return inventory.getShield(); }
  void
  setInventoryItems(const std::array<uint8_t, MAX_INVENTORY_SLOTS> &items) {
    inventory.setItems(items);
  }
  void setEquippedWeapon(uint8_t id) { inventory.setWeapon(id); }
  void setEquippedArmor(uint8_t id) { inventory.setArmor(id); }
  void setEquippedHelmet(uint8_t id) { inventory.setHelmet(id); }
  void setEquippedShield(uint8_t id) { inventory.setShield(id); }

  void resurrect();
  bool assertAttackDistance(int16_t targetX, int16_t targetY) const;
  std::vector<uint8_t> die();
  bool isDead() const { return death; }
};

#endif
