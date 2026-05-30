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

void Player::setPlayerTexture(std::unique_ptr<SDL2pp::Texture> texture) {
  playerTexture = std::move(texture);
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


  if (headTexture) {
    SpriteFrame headSrc = headFrameForDirection(getDirection());
    int headDestW = 24, headDestH = 24;
    int headX = r.x + (r.w - headDestW) / 2 - (direction == Direction::Right ? 1 : 0);
    int headY = r.y - headDestH + 4;
    SDL2pp::Rect headDest{headX, headY, headDestW, headDestH};
    renderer.Copy(*headTexture,
                   SDL2pp::Rect(headSrc.x, headSrc.y, headSrc.w, headSrc.h),
                   headDest);

    if (nameFont && !name.empty()) {
      SDL2pp::Surface surf = nameFont->RenderUTF8_Solid(
          name, SDL_Color{255, 255, 255, 255});
      SDL2pp::Texture tex(renderer, surf);
      int nameX = r.x + (r.w - surf.GetWidth()) / 2;
      int nameY = headY - surf.GetHeight() - 2;
      renderer.Copy(tex, SDL2pp::NullOpt,
                     SDL2pp::Rect(nameX, nameY, surf.GetWidth(), surf.GetHeight()));
    }
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


void Player::setHeadTexture(std::unique_ptr<SDL2pp::Texture> txt) {
  headTexture = std::move(txt);
}

void Player::setNameFont(SDL2pp::Font *font)
{
  nameFont = font;
}
