#ifndef PLAYER_H
#define PLAYER_H

#include "Direction.h"
#include "InventoryConstants.h"
#include "Race.h"
#include <array>
#include <cstdint>
#include <string>

class Player {

public:
  static constexpr int Width = 32;
  static constexpr int Height = 32;
  static constexpr int HeadWidth = 24;
  static constexpr int HeadHeight = 24;

private:
  uint32_t id;
  bool isMoving{false};
  Direction direction{Direction::Down};

  int x, y;
  Race race;
  std::string name;
  std::array<uint8_t, MAX_INVENTORY_SLOTS> inventory{};
  uint8_t equippedWeapon{0};
  uint8_t equippedArmor{0};
  uint8_t equippedHelmet{0};
  uint8_t equippedShield{0};
  uint32_t hp{100};
  uint32_t maxHp{100};
  uint32_t mana{0};
  uint32_t maxMana{0};
  uint32_t gold{0};
  uint32_t level{1};
  uint32_t experience{0};

public:
  Player(uint32_t id, int xOrigin, int yOrigin);

  /* Model, el unico que puede modificar Player */
  uint32_t getID() const;
  void setCoordinates(int x, int y);
  void updateCoordinates(int x, int y, Direction direction);
  void stopMoving();
  void setRace(const Race race);

  void setName(const std::string &v) { name = v; }

  void setHp(uint32_t v) { hp = v; }
  void setMaxHp(uint32_t v) { maxHp = v; }
  void setMana(uint32_t v) { mana = v; }
  void setMaxMana(uint32_t v) { maxMana = v; }
  void setInventory(const std::array<uint8_t, MAX_INVENTORY_SLOTS> &v) {
    inventory = v;
  }
  void setEquippedWeapon(uint8_t v) { equippedWeapon = v; }
  void setEquippedArmor(uint8_t v) { equippedArmor = v; }
  void setEquippedHelmet(uint8_t v) { equippedHelmet = v; }
  void setEquippedShield(uint8_t v) { equippedShield = v; }
  uint8_t getEquippedWeapon() const { return equippedWeapon; }
  uint8_t getEquippedArmor() const { return equippedArmor; }
  uint8_t getEquippedHelmet() const { return equippedHelmet; }
  uint8_t getEquippedShield() const { return equippedShield; }
  const std::array<uint8_t, MAX_INVENTORY_SLOTS> &getInventory() const {
    return inventory;
  }

  void setGold(uint32_t v) { gold = v; }
  void setLevel(uint32_t v) { level = v; }
  void setExperience(uint32_t v) { experience = v; }

  /* View */
  bool getIsMoving() const { return isMoving; }
  Direction getDirection() const { return direction; }
  int get_x() const { return x; }
  int get_y() const { return y; }
  Race getRace() const { return race; }
  const std::string &getName() const { return name; }
  uint32_t getHp() const { return hp; }
  uint32_t getMaxHp() const { return maxHp; }
  uint32_t getMana() const { return mana; }
  uint32_t getMaxMana() const { return maxMana; }
  uint32_t getGold() const { return gold; }
  uint32_t getLevel() const { return level; }
  uint32_t getExperience() const { return experience; }
};

#endif
