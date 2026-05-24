#include "PlayerObserver.h"

PlayerObserver::PlayerObserver() = default;

bool PlayerObserver::getIsMoving() const { return isMoving; }

Direction PlayerObserver::getDirection() const { return direction; }

int PlayerObserver::getX() const { return x; }

int PlayerObserver::getY() const { return y; }

void PlayerObserver::updateDirection(Direction newDirection, bool moving) {
  direction = newDirection;
  isMoving = moving;
}

void PlayerObserver::updatePosition(int newX, int newY) {
  x = newX;
  y = newY;
}
