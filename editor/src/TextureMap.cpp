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
                int id = entry["id"].value_or(0);
                std::string path = entry["path"].value_or("");
                bool transparent = entry["transparent"].value_or(false);

                SDL2pp::Surface surface(path);
                if (transparent) {
                    surface.SetColorKey(true,
                        SDL_MapRGB(surface.Get()->format, 0, 0, 0));
                }

                this->textures.insert({
                    id,
                    {
                        SDL2pp::Texture(renderer, surface),
                        priority 
                    }
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
