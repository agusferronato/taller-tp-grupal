#include "CityEntityRenderable.h"

CityEntityRenderable::CityEntityRenderable(CityEntityModel &entity,
                                           TextureManager &textureManager,
                                           SDL2pp::Font &font, int textureId,
                                           TextureLayoutType layoutType,
                                           const std::string &name)
    : entity(entity), textureManager(textureManager), font(font),
      textureId(textureId), layoutType(layoutType), name(name) {
    Sprite src = textureManager.getBodySprite(layoutType, textureId,
                                              Direction::Down, 0);
    spriteHeight = src.h;
}

void CityEntityRenderable::render(SDL2pp::Renderer &renderer, Camera &camera,
                                  unsigned int it) {
    unsigned int animationIt = entity.getIsMoving() ? it : 0;
    Sprite src = textureManager.getBodySprite(layoutType, textureId,
                                              entity.getDirection(),
                                              animationIt);
    SDL2pp::Rect dst = camera.toScreen(get_x(), get_y(), src.w, src.h);
    renderer.Copy(src.txt, SDL2pp::Rect(src.x, src.y, src.w, src.h), dst);

    SDL2pp::Surface surf =
        font.RenderUTF8_Solid(name, SDL_Color{255, 255, 255, 255});
    SDL2pp::Texture tex(renderer, surf);
    int nameX = dst.x + (dst.w - surf.GetWidth()) / 2;
    int nameY = dst.y - surf.GetHeight() - 2;
    renderer.Copy(tex, SDL2pp::NullOpt,
                  SDL2pp::Rect(nameX, nameY, surf.GetWidth(), surf.GetHeight()));

    wasRendered = true;
}

int CityEntityRenderable::get_x() { return entity.get_x(); }
int CityEntityRenderable::get_y() { return entity.get_y(); }
int CityEntityRenderable::get_h() { return spriteHeight; }
