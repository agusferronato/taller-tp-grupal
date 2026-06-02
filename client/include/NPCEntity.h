#ifndef NPC_ENTITY_H
#define NPC_ENTITY_H

#include "RenderableEntity.h"
#include "NPC.h"
#include "TextureManager.h"
#include "TextureLayoutType.h"

class NPCEntity : public RenderableEntity {

private:
    NPC &npc;
    TextureManager &textureManager;
    int textureId;
    TextureLayoutType layoutType;
    int spriteHeight;

public:

    NPCEntity(NPC &npc, TextureManager &textureManager,
              int textureId, TextureLayoutType layoutType);
    void render(SDL2pp::Renderer &renderer, Camera &camera, unsigned int it) override;
    int get_x() override;
    int get_y() override;
    int get_h() override;
};

#endif
