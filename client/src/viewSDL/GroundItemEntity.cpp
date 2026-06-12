#include "GroundItemEntity.h"

GroundItemEntity::GroundItemEntity(uint8_t itemId, int x, int y,
                                   TextureManager &textureManager)
    : itemId(itemId), x(x), y(y), textureManager(textureManager) {}

void GroundItemEntity::render(SDL2pp::Renderer &renderer, Camera &camera,
                              unsigned int) {
  SDL2pp::Texture *tex = textureManager.getItemIcon(itemId);
  if (!tex)
    return;

  SDL2pp::Rect dst = camera.toScreen(x, y, 32, 32);
  renderer.Copy(*tex, SDL2pp::NullOpt, dst);
  wasRendered = true;
}

int GroundItemEntity::get_x() { return x; }

int GroundItemEntity::get_y() { return y; }

int GroundItemEntity::get_h() { return 32; }
