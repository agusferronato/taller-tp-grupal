#include "NPCEntity.h"

NPCEntity::NPCEntity(NPC &npc, TextureManager &textureManager,
                     int textureId, TextureLayoutType layoutType)
    : npc(npc), textureManager(textureManager),
      textureId(textureId), layoutType(layoutType) {
    Sprite src = textureManager.getBodySprite(layoutType, textureId,
                                               Direction::Down, 0);
    spriteHeight = src.h;
}

void NPCEntity::render(SDL2pp::Renderer &renderer, Camera &camera, unsigned int it) {
    unsigned int animationIt = npc.getIsMoving() ? it : 0;
    Sprite src = textureManager.getBodySprite(layoutType, textureId,
                                               npc.getDirection(), animationIt);
    SDL2pp::Rect dst = camera.toScreen(get_x(), get_y(), src.w, src.h);
    renderer.Copy(src.txt, SDL2pp::Rect(src.x, src.y, src.w, src.h), dst);
    wasRendered = true;
}

int NPCEntity::get_x() { return npc.get_x(); }
int NPCEntity::get_y() { return npc.get_y(); }
int NPCEntity::get_h() { return spriteHeight; }
