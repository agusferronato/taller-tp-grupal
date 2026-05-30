#include "Player.h"

SpriteFrame Player::headFrameForDirection(Direction dir)
{
  int x = 0;
  switch (dir) {
  case Direction::Down:
    x = 0;
    break;
  case Direction::Right:
    x = 17;
    break;
  case Direction::Left:
    x = 34;
    break;
  case Direction::Up:
    x = 51;
    break;
  }
  return {x, 0, 16, 16};
}

Player::Player(uint32_t id, int xOrigin, int yOrigin) : id(id)
{
    x = xOrigin;
    y = yOrigin;
}


void Player::setCoordinates(int x, int y) {
  this->x = x;
  this->y = y;
}

uint32_t Player::getID() const { return id; }

void Player::stopMoving() { isMoving = false; }

void Player::setRace(const std::string &newRace) { race = newRace; }

int Player::get_x() { return x; } 

int Player::get_y() { return y; }

int Player::get_h() { return 32; }


void Player::updateCoordinates(int x, int y, Direction direction) {
  setCoordinates(x, y);
  isMoving = true;
  if (direction != this->direction) {
    this->direction = direction;
  }
}
