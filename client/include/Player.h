#ifndef PLAYER_H
#define PLAYER_H

#include "Direction.h"
#include <cstdint>
#include <memory>

class Player {

private:
  uint32_t id;
  bool isMoving{false};
  Direction direction{Direction::Down};

  int x, y;

public:
  Player(uint32_t id, int xOrigin, int yOrigin);

  /* Model, el unico que puede modificar Player */
  uint32_t getID() const;
  void setCoordinates(int x, int y);
  void updateCoordinates(int x, int y, Direction direction);
  void stopMoving();

  /* View, tiene que ser const el metodo, nunca pasar una referncia solo por
   * copia */
  bool getIsMoving() const { return isMoving; }
  Direction getDirection() const { return direction; }
  int getX() const { return x; }
  int getY() const { return y; }
};

#endif
