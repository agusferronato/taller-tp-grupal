#ifndef PLAYEROBSERVER_H
#define PLAYEROBSERVER_H

#include <string>

#include "Direction.h"

class PlayerObserver {
private:
  bool isMoving{false};
  Direction direction{Direction::Down};
  int x{0};
  int y{0};
  std::string race{"humano"};

public:
  PlayerObserver();
  bool getIsMoving() const;
  Direction getDirection() const;
  int getX() const;
  int getY() const;
  const std::string &getRace() const;

  void updateDirection(Direction direction, bool isMoving);
  void updatePosition(int x, int y);
  void setRace(const std::string &race);
};

#endif
