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

std::map<Direction, std::vector<SpriteData>>
TextureManager::parseBodyFrames(const toml::table& table) {
  std::map<Direction, std::vector<SpriteData>> frames;

  auto parseDir = [&](const std::string& dirName, Direction dir) {
    if (auto* dirTable = table[dirName].as_table()) {
      if (auto* arr = (*dirTable)["frames"].as_array()) {
        std::vector<SpriteData> dirFrames;
        for (auto& elem : *arr) {
          auto& pt = *elem.as_table();
          dirFrames.push_back({pt["x"].value_or(0), pt["y"].value_or(0),
                               pt["w"].value_or(0), pt["h"].value_or(0)});
        }
        frames[dir] = std::move(dirFrames);
      }
    }
  };

  parseDir("Down", Direction::Down);
  parseDir("Up", Direction::Up);
  parseDir("Left", Direction::Left);
  parseDir("Right", Direction::Right);

  return frames;
}

std::map<Direction, SpriteData>
TextureManager::parseHeadFrames(const toml::table& table) {
  std::map<Direction, SpriteData> frames;

  auto parseDir = [&](const std::string& dirName, Direction dir) {
    if (auto* dirTable = table[dirName].as_table()) {
      if (auto* pt = (*dirTable)["point"].as_table()) {
        frames[dir] = {(*pt)["x"].value_or(0), (*pt)["y"].value_or(0),
                       (*pt)["w"].value_or(0), (*pt)["h"].value_or(0)};
      }
    }
  };

  parseDir("Down", Direction::Down);
  parseDir("Up", Direction::Up);
  parseDir("Left", Direction::Left);
  parseDir("Right", Direction::Right);

  return frames;
}

void TextureManager::loadLayoutsFromToml(const std::string &path) {
  toml::table tbl = toml::parse_file(path);

  if (auto *body = tbl["Body"].as_table()) {
    auto frames = parseBodyFrames(*body);
    texturesFrames.erase(TextureLayoutType::Body);
    texturesFrames.emplace(TextureLayoutType::Body, BodyLayout(frames));
  }

  if (auto *head = tbl["Head"].as_table()) {
    auto frames = parseHeadFrames(*head);
    texturesFrames.erase(TextureLayoutType::Head);
    texturesFrames.emplace(TextureLayoutType::Head, HeadLayout(frames));
  }

  auto registerBody = [&](const char* key, TextureLayoutType type) {
    if (auto* t = tbl[key].as_table()) {
      auto frames = parseBodyFrames(*t);
      texturesFrames.erase(type);
      texturesFrames.emplace(type, BodyLayout(frames));
    }
  };

  registerBody("Zombie",         TextureLayoutType::Zombie);
  registerBody("Skeleton",       TextureLayoutType::Skeleton);
  registerBody("Spider",         TextureLayoutType::Spider);
  registerBody("Golem",          TextureLayoutType::Golem);
  registerBody("GreatReamer",    TextureLayoutType::GreatReamer);
  registerBody("Giant",          TextureLayoutType::Giant);
  registerBody("Elf",            TextureLayoutType::Elf);
  registerBody("SpecialSpider",  TextureLayoutType::SpecialSpider);
  registerBody("SpecialSkeleton",TextureLayoutType::SpecialSkeleton);
  registerBody("Orc",            TextureLayoutType::Orc);
}

Sprite TextureManager::getBodySprite(int bodyId, Direction dir,
                                     unsigned int it) {
  
                                      auto &layout =
      std::get<BodyLayout>(texturesFrames.at(TextureLayoutType::Body));
  SpriteData frame = layout.getLayout(dir, it);
  return Sprite{textures.at(bodyId), frame.x, frame.y, frame.w, frame.h};
}

Sprite TextureManager::getBodySprite(TextureLayoutType layoutType, int bodyId,
                                     Direction dir, unsigned int it) {
  auto &layout = std::get<BodyLayout>(texturesFrames.at(layoutType));
  SpriteData frame = layout.getLayout(dir, it);
  return Sprite{textures.at(bodyId), frame.x, frame.y, frame.w, frame.h};
}

Sprite TextureManager::getHeadSprite(int headId, Direction dir) {
  auto &layout =
      std::get<HeadLayout>(texturesFrames.at(TextureLayoutType::Head));
  SpriteData frame = layout.getLayout(dir);
  return Sprite{textures.at(headId), frame.x, frame.y, frame.w, frame.h};
}

Sprite TextureManager::getZombieSprite(int txtID, Direction dir, unsigned int it)
{
  auto& layout = std::get<BodyLayout>(texturesFrames.at(TextureLayoutType::Zombie));

    SpriteData frame = layout.getLayout(dir, it);

    return Sprite{
        textures.at(txtID),
        frame.x,
        frame.y,
        frame.w,
        frame.h
    };
}
