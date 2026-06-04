#ifndef ITEM_TEXTURE_CACHE_H
#define ITEM_TEXTURE_CACHE_H

#include <SDL2pp/SDL2pp.hh>
#include <cstdint>
#include <unordered_map>

class ItemTextureCache {
public:
  void load(SDL2pp::Renderer &renderer);
  SDL2pp::Texture *get(uint8_t itemId) const;

private:
  void tryLoad(SDL2pp::Renderer &renderer, uint8_t itemId,
               const std::string &path);
  std::unordered_map<uint8_t, SDL2pp::Texture> textures;
};

#endif
