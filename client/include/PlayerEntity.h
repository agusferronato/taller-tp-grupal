#ifndef PLAYER_ENTITY_H
#define PLAYER_ENTITY_H

#include <SDL2pp/SDL2pp.hh>

#include "Player.h"
#include "RenderableEntity.h"
#include "TextureManager.h"

class PlayerEntity : public RenderableEntity {
public:
  PlayerEntity(Player &player, TextureManager &textureManager,
               SDL2pp::Font &nameFont);

  void render(SDL2pp::Renderer &renderer, Camera &camera,
              unsigned int it) override;

  Player &getPlayer();
  int get_x() override;
  int get_y() override;
  int get_h() override;

private:
  Player &player;
  TextureManager &textureManager;
  SDL2pp::Font &nameFont;

  void renderBody(SDL2pp::Renderer &renderer, Camera &camera, unsigned int it);
  void renderHead(SDL2pp::Renderer &renderer, Camera &camera);
  void renderName(SDL2pp::Renderer &renderer, Camera &camera);
  int get_head_x(Camera &camera);
  int get_head_y(Camera &camera);
  int getRaceBodyID(Race race) const;
  int getRaceHeadID(Race race) const;
};

#endif
