#ifndef PLAYER_H
#define PLAYER_H

#include "Direction.h"
#include <cstdint>
#include <memory>
#include <string>

class Player {

private:
  uint32_t id;
  bool isMoving{false};
  Direction direction{Direction::Down};

  int x, y;
  std::string race;

public:
  Player(uint32_t id, int xOrigin, int yOrigin);

  uint32_t getID() const;
  void setCoordinates(int x, int y);
  void updateCoordinates(int x, int y, Direction direction);
  void stopMoving();
  void setRace(const std::string &race);

  bool getIsMoving() const { return isMoving; }
  Direction getDirection() const { return direction; }
  int getX() const { return x; }
  int getY() const { return y; }
  const std::string &getRace() const { return race; }
};

#endif
