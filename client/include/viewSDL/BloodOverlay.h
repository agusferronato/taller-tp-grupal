#ifndef BLOOD_OVERLAY_H
#define BLOOD_OVERLAY_H

#include <SDL2pp/SDL2pp.hh>
#include <cstdint>
#include <string>
#include <vector>

class BloodOverlay {
private:
  SDL2pp::Renderer &renderer;
  std::vector<SDL2pp::Texture> textures;

public:
  explicit BloodOverlay(SDL2pp::Renderer &renderer);
  void loadTextures(const std::string &directory);
  void render(uint32_t currentHp, uint32_t maxHp);
};

#endif
