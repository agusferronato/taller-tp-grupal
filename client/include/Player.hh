#ifndef PLAYER_HH
#define PLAYER_HH

#include <SDL2pp/SDL2pp.hh>
#include "PlayerMovedEventDTO.h"

class Player {
    Player(SDL2pp::Texture texture, PlayerMovedEventDTO DTO) : texture(texture) :DTO(DTO) {}
    SDL2pp::Texture texture;
    PlayerMovedEventDTO DTO;
}

#endif