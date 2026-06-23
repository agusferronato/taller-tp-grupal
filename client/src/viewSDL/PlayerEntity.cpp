#include "PlayerEntity.h"

namespace {
bool sameColor(SDL_Color a, SDL_Color b) {
  return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}
} // namespace

PlayerEntity::PlayerEntity(const ClientPlayer &player,
                           TextureManager &textureManager,
                           SDL2pp::Font &nameFont)
    : player(player), textureManager(textureManager), nameFont(nameFont) {}

void PlayerEntity::render(SDL2pp::Renderer &renderer, Camera &camera,
                          unsigned int it) {
  SDL2pp::Rect bounds = camera.toScreen(
      player.get_x() - ClientPlayer::Width,
      player.get_y() - ClientPlayer::HeadHeight - 32, ClientPlayer::Width * 3,
      ClientPlayer::Height + ClientPlayer::HeadHeight + 64);
  if (!camera.isVisibleOnScreen(bounds)) {
    wasRendered = true;
    return;
  }

  if (player.isDead()) {
    renderDead(renderer, camera, it);
  } else {
    renderAlive(renderer, camera, it);
  }
}

void PlayerEntity::renderDead(SDL2pp::Renderer &renderer, Camera &camera,
                              unsigned int it) {
  renderGhostBody(renderer, camera, it);
  renderGhostHead(renderer, camera);
  wasRendered = true;
}

void PlayerEntity::renderAlive(SDL2pp::Renderer &renderer, Camera &camera,
                               unsigned int it) {
  renderBody(renderer, camera, it);
  renderHead(renderer, camera);
  renderEquipable(renderer, camera, it);
  renderName(renderer, camera);
  renderAttackEffect(renderer, camera, it);

  wasRendered = true;
}

void PlayerEntity::renderAttackEffect(SDL2pp::Renderer &renderer,
                                      Camera &camera, unsigned int it) {
  (void)it;

  if (!player.isBeingAttackOrCured()) {
    effectNextFrame = -1;
    return;
  }

  if (effectNextFrame == -1) {
    effectNextFrame = 0;
  }

  auto result = textureManager.getAttackFrame(
      effectParser.getEffectLayout(player.getEffect()),
      effectParser.getEffectID(player.getEffect()), effectNextFrame);

  Sprite &src = result.sprite;

  effectNextFrame++;

  if (effectNextFrame >= result.max_ticks) {
    player.stopAttackEffect();
    effectNextFrame = -1;
    return;
  }

  SDL2pp::Rect dst =
      camera.toScreen(player.get_x() - ClientPlayer::Width,
                      player.get_y() - ClientPlayer::Height / 2, 64, 64);

  if (!camera.isVisibleOnScreen(dst))
    return;

  renderer.Copy(src.txt, SDL2pp::Rect(src.x, src.y, src.w, src.h), dst);
}

void PlayerEntity::renderBody(SDL2pp::Renderer &renderer, Camera &camera,
                              unsigned int it) {
  unsigned int animationIt = player.getIsMoving() ? it : 0;

  int bodyID = getRaceBodyID(player.getRace());
  uint8_t armorSlot = player.getEquippedArmor();

  auto getSprite = [&]() -> Sprite {
    if (armorSlot != 0) {
      try {
        EquipInfo info = equipParser.getInfo(armorSlot);
        if (info.type == "Body") {
          return textureManager.getBodySprite(
              info.textureId, player.getDirection(), animationIt);
        } else {
          return textureManager.getEquipableSprite(
              info.type, info.textureId, player.getDirection(), animationIt);
        }
      } catch (...) {
      }
    }
    return textureManager.getBodySprite(bodyID, player.getDirection(),
                                        animationIt);
  };
  Sprite src = getSprite();
  int ox = (ClientPlayer::Width - src.w) / 2; // centra horizontalmente
  SDL2pp::Rect dst = camera.toScreen(player.get_x() + ox, player.get_y(), src.w,
                                     ClientPlayer::Height);
  if (!camera.isVisibleOnScreen(dst))
    return;

  renderer.Copy(src.txt, SDL2pp::Rect(src.x, src.y, src.w, src.h), dst);
}

void PlayerEntity::renderEquipable(SDL2pp::Renderer &renderer, Camera &camera,
                                   unsigned int it) {
  unsigned int animationIt = player.getIsMoving() ? it : 0;

  auto renderSlot = [&](uint8_t slotItem) {
    if (slotItem == 0)
      return;
    EquipInfo info;
    try {
      info = equipParser.getInfo(slotItem);
    } catch (...) {
      return;
    }

    Sprite src = textureManager.getEquipableSprite(
        info.type, info.textureId, player.getDirection(), animationIt);

    int finalW = (info.renderWidth != -1) ? info.renderWidth : src.w;
    int finalH = (info.renderHeight != -1) ? info.renderHeight : src.h;

    SDL2pp::Rect dst =
        camera.toScreen(player.get_x() + info.offsetX,
                        player.get_y() + info.offsetY, finalW, finalH);
    if (!camera.isVisibleOnScreen(dst))
      return;

    renderer.Copy(src.txt, SDL2pp::Rect(src.x, src.y, src.w, src.h), dst);
  };

  renderSlot(player.getEquippedHelmet());
  renderSlot(player.getEquippedShield());
  renderSlot(player.getEquippedWeapon());
}

void PlayerEntity::renderName(SDL2pp::Renderer &renderer, Camera &camera) {
  // No renderizar si el nombre está vacío
  const std::string &name = player.getName();
  if (name.empty()) {
    return;
  }

  int head_y = get_head_y(camera);
  SDL2pp::Rect playerPosition =
      camera.toScreen(player.get_x(), player.get_y(), ClientPlayer::Width,
                      ClientPlayer::Height);

  const SDL_Color color{255, 255, 255, 255};
  if (!cachedNameTexture || cachedNameText != name ||
      !sameColor(cachedNameColor, color) || cachedNameFont != &nameFont) {
    SDL2pp::Surface surf = nameFont.RenderUTF8_Solid(name, color);
    cachedNameTexture = std::make_unique<SDL2pp::Texture>(renderer, surf);
    cachedNameText = name;
    cachedNameColor = color;
    cachedNameFont = &nameFont;
    cachedNameW = surf.GetWidth();
    cachedNameH = surf.GetHeight();
  }

  int nameX = playerPosition.x + (playerPosition.w - cachedNameW) / 2;
  int nameY = head_y - cachedNameH - 2;
  SDL2pp::Rect dst(nameX, nameY, cachedNameW, cachedNameH);
  if (!camera.isVisibleOnScreen(dst))
    return;

  renderer.Copy(*cachedNameTexture, SDL2pp::NullOpt, dst);
}

void PlayerEntity::renderHead(SDL2pp::Renderer &renderer, Camera &camera) {
  Sprite src = textureManager.getHeadSprite(getRaceHeadID(player.getRace()),
                                            player.getDirection());

  int head_x = get_head_x(camera);
  int head_y = get_head_y(camera);

  SDL2pp::Rect dst{head_x, head_y, ClientPlayer::HeadWidth,
                   ClientPlayer::HeadHeight};
  if (!camera.isVisibleOnScreen(dst))
    return;

  renderer.Copy(src.txt, SDL2pp::Rect(src.x, src.y, src.w, src.h), dst);
}

int PlayerEntity::get_head_x(Camera &camera) {
  SDL2pp::Rect playerPosition =
      camera.toScreen(player.get_x(), player.get_y(), ClientPlayer::Width,
                      ClientPlayer::Height);
  return playerPosition.x + (playerPosition.w - ClientPlayer::HeadWidth) / 2 -
         (player.getDirection() == Direction::Right ? 1 : 0);
}

int PlayerEntity::get_head_y(Camera &camera) {
  SDL2pp::Rect playerPosition =
      camera.toScreen(player.get_x(), player.get_y(), ClientPlayer::Width,
                      ClientPlayer::Height);
  int headY = playerPosition.y - ClientPlayer::HeadHeight + 4;
  return headY;
}

const ClientPlayer &PlayerEntity::getPlayer() { return player; }

int PlayerEntity::get_x() { return player.get_x(); }

int PlayerEntity::get_y() { return player.get_y(); }

int PlayerEntity::get_h() { return ClientPlayer::Height; }

int PlayerEntity::getRaceBodyID(Race race) const {
  switch (race) {
  case Race::Human:
  case Race::Elf:
  case Race::Dwarf:
  case Race::Gnome:
  default:
    return 0;
  }
}

int PlayerEntity::getRaceHeadID(Race race) const {
  switch (race) {
  case Race::Human:
    return 1;
  case Race::Elf:
    return 2;
  case Race::Dwarf:
    return 3;
  case Race::Gnome:
    return 4;
  default:
    return 1;
  }
}

void PlayerEntity::renderGhostBody(SDL2pp::Renderer &renderer, Camera &camera,
                                   unsigned int it) {

  unsigned int animationIt = player.getIsMoving() ? it : 0;

  Sprite src = textureManager.getBodySprite(TextureLayoutType::Ghost, 535,
                                            player.getDirection(), animationIt);

  int ox = (ClientPlayer::Width - src.w) / 2;
  SDL2pp::Rect dst = camera.toScreen(player.get_x() + ox, player.get_y(), src.w,
                                     ClientPlayer::Height);
  if (!camera.isVisibleOnScreen(dst))
    return;

  renderer.Copy(src.txt, SDL2pp::Rect(src.x, src.y, src.w, src.h), dst);
}

void PlayerEntity::renderGhostHead(SDL2pp::Renderer &renderer, Camera &camera) {
  Direction playerDirection = player.getDirection();

  Sprite src = textureManager.getHeadSprite(534, playerDirection);

  int head_x = get_head_x(camera);
  int head_y = get_head_y(camera);

  if (playerDirection == Direction::Left)
    head_x -= 5;
  if (playerDirection == Direction::Right)
    head_x += 5;

  SDL2pp::Rect dst{head_x, head_y, ClientPlayer::HeadWidth,
                   ClientPlayer::HeadHeight};
  if (!camera.isVisibleOnScreen(dst))
    return;

  renderer.Copy(src.txt, SDL2pp::Rect(src.x, src.y, src.w, src.h), dst);
}
