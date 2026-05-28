#include "Player.h"

Player::Player(uint32_t id, int xOrigin, int yOrigin) : id(id) {
  x = xOrigin;
  y = yOrigin;
}

void Player::setCoordinates(int x, int y) {
  this->x = x;
  this->y = y;
}

uint32_t Player::getID() const { return id; }

void Player::stopMoving() { isMoving = false; }

void Player::render(SDL2pp::Renderer &renderer, Camera& camera, unsigned int it) {
  
  const int animationIt = this->getIsMoving() ? static_cast<int>(it) : 0;
  SpriteFrame src =
      spriteFrameCalculator.getSprite(direction, animationIt);

  SDL2pp::Rect r = camera.toScreen(x, y, 32, 32);
  renderer.Copy(*defaultPlayerTexture,
                 SDL2pp::Rect(src.x, src.y, src.w, src.h), r);

  wasRendered = true;
}


int Player::get_x() {
  return x;
}

int Player::get_y() {
  return y;
}

bool Player::rendered() {
  return wasRendered;
}

void Player::updateCoordinates(int x, int y, Direction direction) {
  setCoordinates(x, y);
  isMoving = true;
  if (direction != this->direction) {
    this->direction = direction;
  }
}
