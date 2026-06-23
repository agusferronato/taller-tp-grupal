#ifndef PLAYER_ENTITY_H
#define PLAYER_ENTITY_H

#include <SDL2pp/SDL2pp.hh>
#include <memory>
#include <string>

#include "ClientPlayer.h"
#include "EffectParser.h"
#include "EquipParser.h"
#include "RenderableEntity.h"
#include "TextureManager.h"

class PlayerEntity : public RenderableEntity {
public:
  PlayerEntity(const ClientPlayer &player, TextureManager &textureManager,
               SDL2pp::Font &nameFont);

  void render(SDL2pp::Renderer &renderer, Camera &camera,
              unsigned int it) override;

  const ClientPlayer &getPlayer();
  int get_x() override;
  int get_y() override;
  int get_h() override;

private:
  int effectNextFrame{-1};
  const ClientPlayer &player;
  TextureManager &textureManager;
  SDL2pp::Font &nameFont;
  EquipParser equipParser;
  EffectParser effectParser;
  std::unique_ptr<SDL2pp::Texture> cachedNameTexture;
  std::string cachedNameText;
  SDL_Color cachedNameColor{};
  SDL2pp::Font *cachedNameFont{nullptr};
  int cachedNameW{0};
  int cachedNameH{0};

  void renderAlive(SDL2pp::Renderer &renderer, Camera &camera, unsigned int it);
  void renderDead(SDL2pp::Renderer &renderer, Camera &camera, unsigned int it);

  void renderBody(SDL2pp::Renderer &renderer, Camera &camera, unsigned int it);
  void renderHead(SDL2pp::Renderer &renderer, Camera &camera);
  void renderName(SDL2pp::Renderer &renderer, Camera &camera);
  void renderEquipable(SDL2pp::Renderer &renderer, Camera &camera,
                       unsigned int it);
  void renderGhostBody(SDL2pp::Renderer &renderer, Camera &camera,
                       unsigned int it);
  void renderGhostHead(SDL2pp::Renderer &renderer, Camera &camera);
  int get_head_x(Camera &camera);
  int get_head_y(Camera &camera);
  void renderAttackEffect(SDL2pp::Renderer &renderer, Camera &camera,
                          unsigned int it);
  int getRaceBodyID(Race race) const;
  int getRaceHeadID(Race race) const;
};

#endif
