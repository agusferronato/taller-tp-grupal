#ifndef PLAYER_H
#define PLAYER_H

#include "Camera.h"
#include "Direction.h"
#include "SpriteCalculator.h"
#include <SDL2pp/SDL2pp.hh>
#include <string>

class Player {

private:
  uint32_t id;
  bool isMoving{false};
  Direction direction{Direction::Down};

  int x, y;
  SpriteFrame frame{};
  SpriteFrameCalculator spriteFrameCalculator;
  SDL2pp::Texture texture;
  SDL2pp::Texture headTexture;
  std::string race;
  unsigned int it_init;

  static SpriteFrame headFrameForDirection(Direction dir) {
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

  static int headCenteringOffset(Direction dir) {
    switch (dir) {
    case Direction::Down:
      return 0;
    case Direction::Right:
      return 0;
    case Direction::Left:
      return -1;
    case Direction::Up:
      return 0;
    }
    return 0;
  }

public:
  Player(SDL2pp::Renderer &renderer, uint32_t id,
         const std::string &texturePath, int xOrigin, int yOrigin,
         const std::string &race = "humano");

  int getX() const { return x; }
  int getY() const { return y; }
  uint32_t getID() const { return id; }
  SDL2pp::Texture &getTexture() { return texture; }
  SpriteFrame &getFrame() { return frame; }
  SDL2pp::Texture &getHeadTexture() { return headTexture; }
  SpriteFrame getHeadFrame() const {
    return headFrameForDirection(direction);
  }

  void render(SDL2pp::Renderer &renderer, Camera &camera);

  void setCoordinates(int x, int y) {
    this->x = x;
    this->y = y;
  }

  void updateAnimation(unsigned int it) {

    if (isMoving)
      frame = spriteFrameCalculator.getSprite(direction, it);
    else
      frame = spriteFrameCalculator.getSprite(direction, 0);
  }

  void updateCoordinates(int x, int y, Direction direction) {
    this->x = x;
    this->y = y;
    isMoving = true;
    if (direction != this->direction) {
      this->direction = direction;
    }
  }

  void stopMoving() { isMoving = false; }
};

#endif
