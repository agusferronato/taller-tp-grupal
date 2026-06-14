#include "NPCEntity.h"

NPCEntity::NPCEntity(NPC &npc, TextureManager &textureManager, int textureId,
                     TextureLayoutType layoutType)
    : npc(npc), textureManager(textureManager), textureId(textureId),
      layoutType(layoutType) {
  Sprite src =
      textureManager.getBodySprite(layoutType, textureId, Direction::Down, 0);
  spriteWidth = src.w;
  spriteHeight = src.h;
}

void NPCEntity::render(SDL2pp::Renderer &renderer, Camera &camera,
                       unsigned int it) {
  unsigned int animationIt = npc.getIsMoving() ? it : 0;
  Sprite src = textureManager.getBodySprite(layoutType, textureId,
                                            npc.getDirection(), animationIt);
  SDL2pp::Rect dst = camera.toScreen(get_x(), get_y(), src.w, src.h);
  if (!camera.isVisibleOnScreen(dst)) {
    wasRendered = true;
    return;
  }

  renderer.Copy(src.txt, SDL2pp::Rect(src.x, src.y, src.w, src.h), dst);
  renderAttackEffect(renderer, camera, it);
  wasRendered = true;
}

void NPCEntity::renderAttackEffect(SDL2pp::Renderer &renderer, Camera &camera,
                                   unsigned int it) {
  (void)it;

  if (!npc.isBeingAttacked()) {
    attackNextFrame = -1;
    return;
  }

  if (attackNextFrame == -1) {
    attackNextFrame = 0;
  }

  auto result = textureManager.getAttackFrame(350, attackNextFrame);
  Sprite &src = result.sprite;

  attackNextFrame++;

  const int totalTicks = 24;
  if (attackNextFrame >= totalTicks) {
    npc.stopAttackEffect();
    attackNextFrame = -1;
    return;
  }

  SDL2pp::Rect dst = camera.toScreen(
      get_x() + spriteWidth / 2 - 32, get_y() + spriteHeight / 2 - 32, 64, 64);

  if (!camera.isVisibleOnScreen(dst))
    return;

  renderer.Copy(src.txt, SDL2pp::Rect(src.x, src.y, src.w, src.h), dst);
}

int NPCEntity::get_x() { return npc.get_x(); }
int NPCEntity::get_y() { return npc.get_y(); }
int NPCEntity::get_h() { return spriteHeight; }
