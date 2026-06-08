#ifndef NPC_ENTITY_H
#define NPC_ENTITY_H

#include "NPC.h"
#include "RenderableEntity.h"
#include "TextureLayoutType.h"
#include "TextureManager.h"

class NPCEntity : public RenderableEntity {

private:
  NPC &npc;
  TextureManager &textureManager;
  int textureId;
  TextureLayoutType layoutType;
  int spriteWidth;
  int spriteHeight;
  int attackNextFrame{-1};

public:
  NPCEntity(NPC &npc, TextureManager &textureManager, int textureId,
            TextureLayoutType layoutType);
  void render(SDL2pp::Renderer &renderer, Camera &camera,
              unsigned int it) override;
  int get_x() override;
  int get_y() override;
  int get_h() override;

private:
  void renderAttackEffect(SDL2pp::Renderer &renderer, Camera &camera,
                          unsigned int it);
};

#endif
