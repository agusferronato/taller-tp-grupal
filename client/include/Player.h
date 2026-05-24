#ifndef PLAYER_H
#define PLAYER_H

#include "Direction.h"
#include <cstdint>
#include <memory>
#include <string>

#include "PlayerObserver.h"

class Player {

private:
  uint32_t id;
  bool isMoving{false};
  Direction direction{Direction::Down};

  int x, y;

  PlayerObserver observer;

public:
  Player(uint32_t id, int xOrigin, int yOrigin);

  uint32_t getID() const;
  const PlayerObserver &getObserver() const { return observer; }
  void setCoordinates(int x, int y);
  void updateCoordinates(int x, int y, Direction direction);
  void stopMoving();
};

#endif
