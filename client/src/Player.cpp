#include "Player.h"

Player::Player(uint32_t id, int xOrigin, int yOrigin) : id(id), x(0), y(0) {
  x = xOrigin;
  y = yOrigin;
}

void Player::setCoordinates(int x, int y) {
  this->x = x;
  this->y = y;
}

uint32_t Player::getID() const { return id; }

void Player::stopMoving() { isMoving = false; }

void Player::updateCoordinates(int x, int y, Direction direction) {
  setCoordinates(x, y);
  isMoving = true;
  if (direction != this->direction) {
    this->direction = direction;
  }
}
