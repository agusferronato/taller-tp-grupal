#ifndef CITY_ENTITY_RENDERABLE_H
#define CITY_ENTITY_RENDERABLE_H

#include "CityEntityModel.h"
#include "RenderableEntity.h"
#include "TextureLayoutType.h"
#include "TextureManager.h"
#include <SDL2pp/SDL2pp.hh>

class CityEntityRenderable : public RenderableEntity {
private:
    CityEntityModel &entity;
    TextureManager &textureManager;
    SDL2pp::Font &font;
    int textureId;
    TextureLayoutType layoutType;
    std::string name;
    int spriteHeight;

public:
    CityEntityRenderable(CityEntityModel &entity, TextureManager &textureManager,
                         SDL2pp::Font &font, int textureId,
                         TextureLayoutType layoutType, const std::string &name);
    void render(SDL2pp::Renderer &renderer, Camera &camera,
                unsigned int it) override;
    int get_x() override;
    int get_y() override;
    int get_h() override;
};

#endif
