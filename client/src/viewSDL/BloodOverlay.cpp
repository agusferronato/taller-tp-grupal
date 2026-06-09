#include "BloodOverlay.h"
#include <algorithm>
#include <cmath>

BloodOverlay::BloodOverlay(SDL2pp::Renderer &renderer) : renderer(renderer) {}

void BloodOverlay::loadTextures(const std::string &directory) {
    textures.clear();
    for (int i = 0; i <= 10; ++i) {
        std::string path = directory + std::to_string(i) + ".png";
        SDL2pp::Surface surf(path);
        SDL2pp::Texture tex(renderer, surf);
        textures.push_back(std::move(tex));
    }
}

void BloodOverlay::render(uint32_t currentHp, uint32_t maxHp) {
    if (currentHp >= maxHp || maxHp == 0)
        return;

    float ratio = 1.0f - static_cast<float>(currentHp) /
                              static_cast<float>(maxHp);
    int frame = static_cast<int>(std::round(ratio * 10.0f));
    frame = std::clamp(frame, 0, 10);

    SDL2pp::Texture &tex = textures[frame];
    SDL2pp::Rect dst(0, 0, 960, 540);
    renderer.Copy(tex, SDL2pp::NullOpt, dst);
}
