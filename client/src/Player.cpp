#include "Player.h"

Player::Player(uint32_t id, int xOrigin, int yOrigin) : id(id), x(0), y(0) {
  observer = std::make_unique<PlayerObserver>();
  x = xOrigin;
  y = yOrigin;
  observer->updatePosition(x, y);
  observer->updateDirection(direction, isMoving);
}

void Player::setCoordinates(int x, int y) {
  this->x = x;
  this->y = y;
  observer->updatePosition(x, y);
}

uint32_t Player::getID() const { return id; }

void Player::stopMoving() {
  isMoving = false;
  observer->updateDirection(direction, isMoving);
}

void Player::setRace(const std::string &race) { observer->setRace(race); }

void Player::updateCoordinates(int x, int y, Direction direction) {
  setCoordinates(x, y);
  isMoving = true;
  if (direction != this->direction) {
    this->direction = direction;
  }
  observer->updateDirection(this->direction, isMoving);
}
