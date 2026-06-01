#include "TextureManager.h"

#include <SDL2/SDL.h>
#include <toml++/toml.hpp>

#include "BodyLayout.h"
#include "HeadLayout.h"

TextureManager::TextureManager(SDL2pp::Renderer &renderer)
    : renderer(renderer) {}

void TextureManager::loadTexturesFromToml(const std::string &path) {
  toml::table tbl = toml::parse_file(path);

  if (auto *sprites = tbl["sprites"].as_array()) {
    for (auto &elem : *sprites) {
      auto &entry = *elem.as_table();

      int id = entry["id"].value_or(0);
      std::string texPath = entry["path"].value_or("");
      bool transparent = entry["transparent"].value_or(false);

      SDL2pp::Surface surface(texPath);
      if (transparent) {
        surface.SetColorKey(true, SDL_MapRGB(surface.Get()->format, 0, 0, 0));
      }
      SDL2pp::Texture txt(renderer, surface);
      textures.emplace(id, std::move(txt));
    }
  }
}

void TextureManager::loadLayoutsFromToml(const std::string &path) {
  toml::table tbl = toml::parse_file(path);

  if (auto *body = tbl["Body"].as_table()) {
    std::map<Direction, std::vector<SpriteData>> bodyFrames;
    auto parseDir = [&](const std::string &dirName, Direction dir) {
      if (auto *dirTable = (*body)[dirName].as_table()) {
        if (auto *arr = (*dirTable)["frames"].as_array()) {
          std::vector<SpriteData> frames;
          for (auto &elem : *arr) {
            auto &pt = *elem.as_table();
            frames.push_back({pt["x"].value_or(0), pt["y"].value_or(0),
                              pt["w"].value_or(0), pt["h"].value_or(0)});
          }
          bodyFrames[dir] = std::move(frames);
        }
      }
    };
    parseDir("Down", Direction::Down);
    parseDir("Up", Direction::Up);
    parseDir("Left", Direction::Left);
    parseDir("Right", Direction::Right);
    texturesFrames.erase(TextureLayoutType::Body);
    texturesFrames.emplace(TextureLayoutType::Body, BodyLayout(bodyFrames));
  }

  if (auto *head = tbl["Head"].as_table()) {
    std::map<Direction, SpriteData> headFrames;
    auto addHeadFrame = [&](const std::string &dirName, Direction dir) {
      if (auto *dirTable = (*head)[dirName].as_table()) {
        if (auto *pt = (*dirTable)["point"].as_table()) {
          headFrames[dir] = {(*pt)["x"].value_or(0), (*pt)["y"].value_or(0),
                             (*pt)["w"].value_or(0), (*pt)["h"].value_or(0)};
        }
      }
    };
    addHeadFrame("Down", Direction::Down);
    addHeadFrame("Up", Direction::Up);
    addHeadFrame("Left", Direction::Left);
    addHeadFrame("Right", Direction::Right);
    texturesFrames.erase(TextureLayoutType::Head);
    texturesFrames.emplace(TextureLayoutType::Head, HeadLayout(headFrames));
  }
}

Sprite TextureManager::getBodySprite(uint32_t bodyID, Direction dir,
                                     unsigned int it) {
  auto &layout =
      std::get<BodyLayout>(texturesFrames.at(TextureLayoutType::Body));
  SpriteData frame = layout.getLayout(dir, it);
  return Sprite{textures.at(bodyID), frame.x, frame.y, frame.w, frame.h};
}

Sprite TextureManager::getHeadSprite(uint32_t headID, Direction dir) {
  auto &layout =
      std::get<HeadLayout>(texturesFrames.at(TextureLayoutType::Head));
  SpriteData frame = layout.getLayout(dir);
  return Sprite{textures.at(headID), frame.x, frame.y, frame.w, frame.h};
}
