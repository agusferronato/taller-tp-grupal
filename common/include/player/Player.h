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
public:
  static constexpr uint32_t NO_CLAN = 0;

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
  uint32_t clanId{NO_CLAN};

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
  void heal(uint32_t amount);
  bool useMana(uint32_t amount);
  void addMana(uint32_t amount);
  bool gainExperience(uint32_t amount);
  bool expentGold(uint32_t amount);
  void earnGold(uint32_t);
  void removeGold(uint32_t amount);
  uint32_t attack() const;
  void setLevel(uint32_t level);

  bool isMoving() const;
  Direction getDirection() const;
  int getX() const;
  int getY() const;
  Race getRace() const;
  const std::string &getName() const;
  uint32_t getHp() const;
  uint32_t getMaxHp() const;
  uint32_t getMana() const;
  uint32_t getMaxMana() const;
  uint32_t getGold() const;
  uint32_t getLevel() const;
  uint32_t getExperience() const;
  PlayerClass getPlayerClass() const;
  uint32_t getStrength() const;
  uint32_t getAgility() const;
  uint32_t getConstitution() const;
  uint32_t getIntelligence() const;

  Inventory &getInventory();
  const Inventory &getInventory() const;
  bool addItem(uint8_t itemId);
  bool equipItem(uint8_t slotIndex);
  bool unequipSlot(EquipSlot slot);
  bool removeItem(uint8_t slotIndex);
  std::array<uint8_t, MAX_INVENTORY_SLOTS> getInventoryItems() const;
  const Weapon &getEquippedWeapon() const;
  const Armor &getEquippedArmor() const;
  const Helmet &getEquippedHelmet() const;
  const Shield &getEquippedShield() const;
  void setInventoryItems(const std::array<uint8_t, MAX_INVENTORY_SLOTS> &items);
  void setEquippedWeapon(uint8_t id);
  void setEquippedArmor(uint8_t id);
  void setEquippedHelmet(uint8_t id);
  void setEquippedShield(uint8_t id);

  void resurrect();
  uint32_t getClanId() const;
  bool hasClan() const;
  void joinClan(uint32_t clanId);
  void leaveClan();

  bool assertAttackDistance(int16_t targetX, int16_t targetY) const;
  std::vector<uint8_t> die();
  bool isDead() const;
};

#endif
