#include "TextureMap.h"
#include <toml++/toml.hpp>
#include <QDebug>
#include <string>

TextureMap::TextureMap(SDL2pp::Renderer& renderer) {
    try {
        toml::table tbl = toml::parse_file("assets/textures.toml");
        if (auto texTable = tbl["textures"].as_table()) {
            for (auto& [key, value] : *texTable) {
                auto& entry = *value.as_table();
                TextureData data;
                data.key = std::string(key);
                data.id = entry["id"].value_or(0);
                data.name = entry["name"].value_or("");
                data.path = entry["path"].value_or("");
                data.collidable = entry["collidable"].value_or(false);
                data.transparent = entry["transparent"].value_or(false);
                data.priority = entry["priority"].value_or(0);
                data.collidablePercentage = entry["collidable_percentage"].value_or(1.0f);

                SDL2pp::Surface surface(data.path);
                if (data.transparent) {
                    surface.SetColorKey(true,
                        SDL_MapRGB(surface.Get()->format, 0, 0, 0));
                }

                this->textures.insert({
                    data.id,
                    TextureInMap{data, SDL2pp::Texture(renderer, surface), std::move(surface)}
                });
            }
        }
    } catch (const std::exception& err) {
        qDebug() << "Error loading textures from TOML:" << err.what();
    }
}

TextureInMap& TextureMap::getTexture(int texture_id) {
    return textures.at(texture_id);
}
