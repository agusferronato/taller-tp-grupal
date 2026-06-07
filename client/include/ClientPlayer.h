#ifndef CLIENT_PLAYER_H
#define CLIENT_PLAYER_H

#include "Direction.h"
#include "Inventory.h"
#include "InventoryConstants.h"
#include "Player.h"
#include "Race.h"
#include <array>
#include <cstdint>
#include <string>

class ClientPlayer {

public:
  static constexpr int Width = 20;
  static constexpr int Height = 37;
  static constexpr int HeadWidth = 18;
  static constexpr int HeadHeight = 18;

private:
  uint32_t id;
  Player player;

public:
  ClientPlayer(uint32_t id, std::string name, int xOrigin, int yOrigin,
               Direction direction, const PlayerStatsInfo &statsInfo);

  /* Model, el unico que puede modificar Player */
  uint32_t getID() const;
  void updateCoordinates(int x, int y, Direction direction);
  void stopMoving();
  void updateStats(uint32_t hp, uint32_t maxHp, uint32_t mana, uint32_t maxMana,
                   uint32_t gold, uint32_t level, uint32_t experience);

  void setInventory(const std::array<uint8_t, MAX_INVENTORY_SLOTS> &items) {
    player.setInventoryItems(items);
  }
  void setEquippedWeapon(uint8_t id) { player.setEquippedWeapon(id); }
  void setEquippedArmor(uint8_t id) { player.setEquippedArmor(id); }
  void setEquippedHelmet(uint8_t id) { player.setEquippedHelmet(id); }
  void setEquippedShield(uint8_t id) { player.setEquippedShield(id); }

  /* View */
  bool getIsMoving() const { return player.isMoving(); }
  Direction getDirection() const { return player.getDirection(); }
  int get_x() const { return player.getX(); }
  int get_y() const { return player.getY(); }
  Race getRace() const { return player.getRace(); }
  const std::string &getName() const { return player.getName(); }
  uint32_t getHp() const { return player.getHp(); }
  uint32_t getMaxHp() const { return player.getMaxHp(); }
  uint32_t getMana() const { return player.getMana(); }
  uint32_t getMaxMana() const { return player.getMaxMana(); }
  uint32_t getGold() const { return player.getGold(); }
  uint32_t getLevel() const { return player.getLevel(); }
  uint32_t getExperience() const { return player.getExperience(); }

  const Player &getPlayer() const { return player; }

  uint8_t getEquippedWeapon() const { return player.getEquippedWeapon(); }
  uint8_t getEquippedArmor() const { return player.getEquippedArmor(); }
  uint8_t getEquippedHelmet() const { return player.getEquippedHelmet(); }
  uint8_t getEquippedShield() const { return player.getEquippedShield(); }
  const Inventory &getInventory() const { return player.getInventory(); }
};

#endif
