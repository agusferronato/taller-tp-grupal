#ifndef PLAYEROBSERVER_H
#define PLAYEROBSERVER_H

#include "Direction.h"

class PlayerObserver {
private:
  bool isMoving{false};
  Direction direction{Direction::Down};
  int x{0};
  int y{0};

public:
  PlayerObserver();
  bool getIsMoving() const;
  Direction getDirection() const;
  int getX() const;
  int getY() const;

  void updateDirection(Direction direction, bool isMoving);
  void updatePosition(int x, int y);
};

#endif
