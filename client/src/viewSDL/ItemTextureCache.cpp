#include "ItemTextureCache.h"

void ItemTextureCache::load(SDL2pp::Renderer &renderer) {
  tryLoad(renderer, 1, "assets/items/espada.png");
  tryLoad(renderer, 2, "assets/items/hacha.png");
  tryLoad(renderer, 3, "assets/items/martillo.png");
  tryLoad(renderer, 4, "assets/items/arcoSimple.png");
  tryLoad(renderer, 5, "assets/items/arcoComp.png");
  tryLoad(renderer, 12, "assets/items/tunica.png");
  tryLoad(renderer, 16, "assets/items/escudo1.png");
  tryLoad(renderer, 17, "assets/items/escudo2.png");
  tryLoad(renderer, 18, "assets/items/pocion1.png");
  tryLoad(renderer, 19, "assets/items/pocion2.png");
}

void ItemTextureCache::tryLoad(SDL2pp::Renderer &renderer, uint8_t itemId,
                                const std::string &path) {
  try {
    SDL2pp::Surface surf(path);
    textures.emplace(itemId, SDL2pp::Texture(renderer, surf));
  } catch (...) {
  }
}

SDL2pp::Texture *ItemTextureCache::get(uint8_t itemId) const {
  auto it = textures.find(itemId);
  if (it != textures.end())
    return const_cast<SDL2pp::Texture *>(&it->second);
  return nullptr;
}
