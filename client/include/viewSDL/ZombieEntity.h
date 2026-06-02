#ifndef ZOMBIE_ENTITY_H
#define ZOMBIE_ENTITY_H

#include "RenderableEntity.h"
#include "TextureManager.h"
#include "Zombie.h"

class ZombieEntity : public RenderableEntity {

private:
  static const int BodyTextureID = 2;

private:
  Zombie &zombie;
  TextureManager &textureManager;

public:
  ZombieEntity(Zombie &zombie, TextureManager &textureManager);

  void render(SDL2pp::Renderer &renderer, Camera &camera,
              unsigned int it) override;

  int get_x() override;
  int get_y() override;
  int get_h() override;
};

#endif
