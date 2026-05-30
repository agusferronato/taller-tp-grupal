#include "Player.h"

SDL2pp::Texture *Player::playerTexture = nullptr;
SpriteFrameCalculator Player::spriteFrameCalculator;

Player::Player(uint32_t id, int xOrigin, int yOrigin) : id(id) {
  x = xOrigin;
  y = yOrigin;
}

void Player::setPlayerTexture(SDL2pp::Texture *texture) {
  playerTexture = texture;
}

void Player::setCoordinates(int x, int y) {
  this->x = x;
  this->y = y;
}

uint32_t Player::getID() const { return id; }

void Player::stopMoving() { isMoving = false; }

void Player::setRace(const std::string &newRace) { race = newRace; }

void Player::render(SDL2pp::Renderer &renderer, Camera &camera,
                    unsigned int it) {

  unsigned int animationIt = this->getIsMoving() ? it : 0;


  SpriteFrame src = spriteFrameCalculator.getSprite(direction, animationIt);

  SDL2pp::Rect r = camera.toScreen(x, y, 32, 32);
  if (playerTexture) {
    renderer.Copy(*playerTexture, SDL2pp::Rect(src.x, src.y, src.w, src.h), r);
  }

  wasRendered = true;
}

int Player::get_x() { return x; }

int Player::get_y() { return y; }

int Player::get_h() { return 32; }

bool Player::rendered() { return wasRendered; }

void Player::updateCoordinates(int x, int y, Direction direction) {
  setCoordinates(x, y);
  isMoving = true;
  if (direction != this->direction) {
    this->direction = direction;
  }
}
