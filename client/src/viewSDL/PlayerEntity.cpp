#include "PlayerEntity.h"

PlayerEntity::PlayerEntity(const ClientPlayer &player,
                           TextureManager &textureManager,
                           SDL2pp::Font &nameFont)
    : player(player), textureManager(textureManager), nameFont(nameFont) {}

struct EquipInfo {
  std::string type;
  int textureId;
};

static const std::map<uint8_t, EquipInfo> equipMap = {
    {1, {"EquipableGrid", 200}},  {2, {"EquipableGrid", 201}},
    {3, {"EquipableGrid", 202}},  {4, {"EquipableGrid", 203}},
    {5, {"EquipableGrid", 204}},  {6, {"EquipableGrid", 205}},
    {7, {"EquipableGrid", 206}},  {8, {"EquipableGrid", 207}},
    {9, {"EquipableGrid", 208}},  {10, {"Body", 215}},
    {11, {"Body", 216}}, {12, {"Tunic", 209}},
    {13, {"Helmet", 210}},        {14, {"Helmet", 211}},
    {15, {"Helmet", 212}},        {16, {"EquipableGrid", 214}},
    {17, {"EquipableGrid", 213}},
};

void PlayerEntity::render(SDL2pp::Renderer &renderer, Camera &camera,
                          unsigned int it) {
  renderBody(renderer, camera, it);
  renderHead(renderer, camera);
  renderEquipable(renderer, camera, it);
  renderName(renderer, camera);

  wasRendered = true;
}

void PlayerEntity::renderBody(SDL2pp::Renderer &renderer, Camera &camera,
                              unsigned int it) {
  unsigned int animationIt = player.getIsMoving() ? it : 0;

  int bodyID = getRaceBodyID(player.getRace());
  uint8_t armorSlot = player.getEquippedArmor();
  auto itMap = equipMap.find(armorSlot);

  auto getSprite = [&]() -> Sprite {
    if (armorSlot != 0 && itMap != equipMap.end()) {
      if (itMap->second.type == "Body") {
        return textureManager.getBodySprite(
            itMap->second.textureId, player.getDirection(), animationIt);
      } else {
        return textureManager.getEquipableSprite(
            itMap->second.type, itMap->second.textureId, player.getDirection(),
            animationIt);
      }
    } else {
      return textureManager.getBodySprite(bodyID, player.getDirection(),
                                          animationIt);
    }
  };

  Sprite src = getSprite();

  SDL2pp::Rect dst =
      camera.toScreen(player.get_x(), player.get_y(), ClientPlayer::Width,
                      ClientPlayer::Height);
  renderer.Copy(src.txt, SDL2pp::Rect(src.x, src.y, src.w, src.h), dst);
}

void PlayerEntity::renderEquipable(SDL2pp::Renderer &renderer, Camera &camera,
                                   unsigned int it) {
  unsigned int animationIt = player.getIsMoving() ? it : 0;

  auto renderSlot = [&](uint8_t slotItem, int offsetX = 0, int offsetY = 0,
                        int width = -1, int height = -1) {
    if (slotItem == 0)
      return;
    auto itMap = equipMap.find(slotItem);
    if (itMap == equipMap.end())
      return;

    Sprite src = textureManager.getEquipableSprite(
        itMap->second.type, itMap->second.textureId, player.getDirection(),
        animationIt);

    int finalW = (width != -1) ? width : src.w;
    int finalH = (height != -1) ? height : src.h;

    SDL2pp::Rect dst = camera.toScreen(player.get_x() + offsetX,
                                       player.get_y() + offsetY, finalW, finalH);
    renderer.Copy(src.txt, SDL2pp::Rect(src.x, src.y, src.w, src.h), dst);
  };

  renderSlot(player.getEquippedHelmet(), 3.5, -22, 26, 65);
  renderSlot(player.getEquippedShield(), 8);
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

  SDL2pp::Surface surf =
      nameFont.RenderUTF8_Solid(name, SDL_Color{255, 255, 255, 255});
  SDL2pp::Texture tex(renderer, surf);
  int nameX = playerPosition.x + (playerPosition.w - surf.GetWidth()) / 2;
  int nameY = head_y - surf.GetHeight() - 2;
  renderer.Copy(tex, SDL2pp::NullOpt,
                SDL2pp::Rect(nameX, nameY, surf.GetWidth(), surf.GetHeight()));
}

void PlayerEntity::renderHead(SDL2pp::Renderer &renderer, Camera &camera) {
  Sprite src = textureManager.getHeadSprite(getRaceHeadID(player.getRace()),
                                            player.getDirection());

  int head_x = get_head_x(camera);
  int head_y = get_head_y(camera);

  SDL2pp::Rect dst{head_x, head_y, ClientPlayer::HeadWidth,
                   ClientPlayer::HeadHeight};
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
