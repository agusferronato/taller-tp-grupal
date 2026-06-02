#include "PlayerEntity.h"

PlayerEntity::PlayerEntity(const Player &player, TextureManager &textureManager,
                           SDL2pp::Font &nameFont)
    : player(player), textureManager(textureManager), nameFont(nameFont) {}

void PlayerEntity::render(SDL2pp::Renderer &renderer, Camera &camera,
                          unsigned int it) {
  renderBody(renderer, camera, it);
  renderHead(renderer, camera);
  renderName(renderer, camera);

  wasRendered = true;
}

void PlayerEntity::renderBody(SDL2pp::Renderer &renderer, Camera &camera,
                              unsigned int it) {
  unsigned int animationIt = player.getIsMoving() ? it : 0;

  Sprite src = textureManager.getBodySprite(getRaceBodyID(player.getRace()),
                                            player.getDirection(), animationIt);

  SDL2pp::Rect playerRect = camera.toScreen(get_x(), get_y(), Player::Width, Player::Height);
  SDL2pp::Rect dst{playerRect.x + (Player::Width - src.w) / 2,
                   playerRect.y + (Player::Height - src.h) / 2,
                   src.w, src.h};

  renderer.Copy(src.txt, SDL2pp::Rect(src.x, src.y, src.w, src.h), dst);
}

void PlayerEntity::renderName(SDL2pp::Renderer &renderer, Camera &camera) {
  // No renderizar si el nombre está vacío
  const std::string &name = player.getName();
  if (name.empty()) {
    return;
  }

  int head_y = get_head_y(camera);
  SDL2pp::Rect playerPosition = camera.toScreen(player.get_x(), player.get_y(),
                                                Player::Width, Player::Height);

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

  SDL2pp::Rect dst{head_x + (Player::HeadWidth - src.w) / 2,
                   head_y + (Player::HeadHeight - src.h) / 2,
                   src.w, src.h};

  renderer.Copy(src.txt, SDL2pp::Rect(src.x, src.y, src.w, src.h), dst);
}

int PlayerEntity::get_head_x(Camera &camera) {
  SDL2pp::Rect playerPosition = camera.toScreen(player.get_x(), player.get_y(),
                                                Player::Width, Player::Height);
  return playerPosition.x + (playerPosition.w - Player::HeadWidth) / 2 -
         (player.getDirection() == Direction::Right ? 1 : 0);
}

int PlayerEntity::get_head_y(Camera &camera) {
  SDL2pp::Rect playerPosition = camera.toScreen(player.get_x(), player.get_y(),
                                                Player::Width, Player::Height);
  int headY = playerPosition.y - Player::HeadHeight + 5;
  return headY;
}

const Player &PlayerEntity::getPlayer() { return player; }

int PlayerEntity::get_x() { return player.get_x(); }

int PlayerEntity::get_y() { return player.get_y(); }

int PlayerEntity::get_h() { return Player::Height; }

int PlayerEntity::getRaceBodyID(Race race) const {
  // TODO: Agregar texturas para los cuerpos de las otras razas
  // Por ahora todas usan el mismo body (ID 0)
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
    return 16;
  case Race::Elf:
    return 17;
  case Race::Dwarf:
    return 14;
  case Race::Gnome:
    return 15;
  default:
    return 16; // Human como fallback
  }
}
