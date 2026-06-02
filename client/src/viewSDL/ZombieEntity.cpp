#include "ZombieEntity.h"

ZombieEntity::ZombieEntity(Zombie &zombie, TextureManager &textureManager)
    : zombie(zombie), textureManager(textureManager) {}

void ZombieEntity::render(SDL2pp::Renderer &renderer, Camera &camera,
                          unsigned int it) {
  unsigned int animationIt = zombie.getIsMoving() ? it : 0;

  Sprite src = textureManager.getZombieSprite(
      ZombieEntity::BodyTextureID, zombie.getDirection(), animationIt);

  SDL2pp::Rect dst = camera.toScreen(get_x(), get_y(), src.w, src.h);
  renderer.Copy(src.txt, SDL2pp::Rect(src.x, src.y, src.w, src.h), dst);

  wasRendered = true;
}

int ZombieEntity::get_x() { return zombie.get_x(); }

int ZombieEntity::get_y() { return zombie.get_y(); }

int ZombieEntity::get_h() { return zombie.get_h(); }
