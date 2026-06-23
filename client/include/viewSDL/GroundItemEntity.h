#ifndef GROUND_ITEM_ENTITY_H
#define GROUND_ITEM_ENTITY_H

#include "RenderableEntity.h"
#include "TextureManager.h"

class GroundItemEntity : public RenderableEntity {
public:
  GroundItemEntity(uint8_t itemId, int x, int y,
                   TextureManager &textureManager);

  void render(SDL2pp::Renderer &renderer, Camera &camera,
              unsigned int it) override;
  int get_x() override;
  int get_y() override;
  int get_h() override;

private:
  uint8_t itemId;
  int x, y;
  TextureManager &textureManager;
};

#endif
