#ifndef PLAYER_H
#define PLAYER_H

#include "Direction.h"
#include "RenderableEntity.h"
#include "Camera.h"
#include <cstdint>
#include <memory>
#include <SpriteCalculator.h>
#include <SDL2pp/SDL2pp.hh>

class Player : public RenderableEntity {

private:
  uint32_t id;
  bool isMoving{false};
  Direction direction{Direction::Down};

  int x, y;

  static SDL2pp::Texture *playerTexture;
  static SpriteFrameCalculator spriteFrameCalculator;

public:
  Player(uint32_t id, int xOrigin, int yOrigin);

  static void setPlayerTexture(SDL2pp::Texture *texture);

  /* Model, el unico que puede modificar Player */
  uint32_t getID() const;
  void setCoordinates(int x, int y);
  void updateCoordinates(int x, int y, Direction direction);
  void stopMoving();

  /* View */
  void render(SDL2pp::Renderer &renderer, Camera &camera,
              unsigned int it) override;

  int get_x() override;
  int get_y() override;

  int get_h() override;

  bool rendered() override;

  /* View, tiene que ser const el metodo, nunca pasar una referncia solo por
   * copia */
  bool getIsMoving() const { return isMoving; }
  Direction getDirection() const { return direction; }
};

#endif
