#ifndef CLIENT_PLAYER_H
#define CLIENT_PLAYER_H

#include "Direction.h"
#include "EffectType.h"
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
  mutable bool beingAttacked{false};
  mutable EffectType currentEffect{EffectType::NormalAttack};

public:
  ClientPlayer(uint32_t id, std::string name, int xOrigin, int yOrigin,
               Direction direction, const PlayerStatsInfo &statsInfo);

  /* Model, el unico que puede modificar Player */
  uint32_t getID() const;
  void updateCoordinates(int x, int y, Direction direction);
  void stopMoving();
  void updateStats(uint32_t hp, uint32_t maxHp, uint32_t mana, uint32_t maxMana,
                   uint32_t gold, uint32_t level, uint32_t experience);

  void setInventory(const std::array<uint8_t, MAX_INVENTORY_SLOTS> &items);
  void setEquippedWeapon(uint8_t id);
  void setEquippedArmor(uint8_t id);
  void setEquippedHelmet(uint8_t id);
  void setEquippedShield(uint8_t id);

  /* View */
  bool getIsMoving() const;
  Direction getDirection() const;
  int get_x() const;
  int get_y() const;
  Race getRace() const;
  const std::string &getName() const;
  uint32_t getHp() const;
  uint32_t getMaxHp() const;
  uint32_t getMana() const;
  uint32_t getMaxMana() const;
  uint32_t getGold() const;
  uint32_t getLevel() const;
  uint32_t getExperience() const;

  const Player &getPlayer() const;

  bool isBeingAttackOrCured() const;
  EffectType getEffect() const;
  void setBeingAttacked(bool v, EffectType effect = EffectType::NormalAttack);
  void stopAttackEffect() const;

  uint8_t getEquippedWeapon() const;
  uint8_t getEquippedArmor() const;
  uint8_t getEquippedHelmet() const;
  uint8_t getEquippedShield() const;
  const Inventory &getInventory() const;
  void die();
  void resurrect(int x, int y);
  bool isDead() const;
};

#endif
