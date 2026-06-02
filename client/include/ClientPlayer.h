#ifndef CLIENT_PLAYER_H
#define CLIENT_PLAYER_H

#include "Direction.h"
#include "InventoryConstants.h"
#include "Player.h"
#include "Race.h"
#include <array>
#include <cstdint>
#include <string>

class ClientPlayer {

public:
  static constexpr int Width = 32;
  static constexpr int Height = 32;
  static constexpr int HeadWidth = 24;
  static constexpr int HeadHeight = 24;

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
};

#endif
