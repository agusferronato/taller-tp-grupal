#ifndef PLAYER_ENTITY_H
#define PLAYER_ENTITY_H


#include "RenderableEntity.h"
#include "Player.h"


class PlayerEntity : public RenderableEntity {

private:
    Player& player;
    TextureManager& textureManager;

public:

    PlayerEntity(Player& player, TextureManager& textureManager);

    void render(SDL2pp::Renderer& renderer, Camera& camera, unsigned int it) override;


    int get_x() override;
    int get_y() override;
    int get_h() override;

private:

    void renderBody(SDL2pp::Renderer &renderer, Camera &camera, unsigned int it);

};



#endif
