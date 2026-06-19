#include "TextureMapper.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>
#include <toml++/toml.hpp>

TextureMapper::TextureMapper(SDL2pp::Renderer &renderer) : renderer(renderer) {}

void TextureMapper::loadFromToml(const std::string &path) {
  toml::table tbl = toml::parse_file(path);
  if (auto *texTable = tbl["textures"].as_table()) {
    for (auto &[key, value] : *texTable) {
      auto &entry = *value.as_table();
      int id = entry["id"].value_or(0);
      std::string texPath = entry["path"].value_or("");
      bool transparent = entry["transparent"].value_or(false);

      SDL2pp::Surface surface(texPath);
      if (transparent) {
        surface.SetColorKey(true, SDL_MapRGB(surface.Get()->format, 0, 0, 0));
      }

      SDL2pp::Texture txt(renderer, surface);
      int w = txt.GetWidth();
      int h = txt.GetHeight();
      textures.emplace(id, TextureInfo{std::move(txt), w, h});
    }
  }
}

void TextureMapper::buildRenderGrid(const std::list<TileOrigin> &origins,
                                    int gridSizePx) {
  int maxPriority = 0;
  for (const auto &origin : origins) {
    if (origin.priority > maxPriority)
      maxPriority = origin.priority;
  }
  tilesToRender.resize(maxPriority + 1);

  for (const auto &origin : origins) {
    auto it = textures.find(origin.texture_id);
    if (it == textures.end())
      continue;

    int rows = std::ceil(static_cast<float>(it->second.height) / gridSizePx);
    int columns = std::ceil(static_cast<float>(it->second.width) / gridSizePx);

    TextureRenderInfo info;
    info.texture_id = origin.texture_id;
    info.origin_i = origin.x;
    info.origin_j = origin.y;
    info.width_px = it->second.width;
    info.height_px = it->second.height;

    auto key = std::make_pair(origin.y + rows - 1, origin.x + columns - 1);
    tilesToRender[origin.priority][key] = info;
  }
}

SDL2pp::Texture &TextureMapper::getTexture(int id) {
  SDL2pp::Texture& texture = textures.at(id).texture;
  return texture;
}

const std::vector<std::map<std::pair<int, int>, TextureRenderInfo>> &
TextureMapper::getTilesToRender() const {
  return tilesToRender;
}
