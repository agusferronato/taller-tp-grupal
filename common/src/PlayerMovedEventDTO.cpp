#include "PlayerMovedEventDTO.h"

PlayerMovedEventDTO::PlayerMovedEventDTO(PlayerId player_id, int16_t x, int16_t y, Direction direction)
    : player_id(player_id), x(x), y(y), direction(direction) {}

PlayerId PlayerMovedEventDTO::getPlayerId() const { return player_id; }

int16_t PlayerMovedEventDTO::getX() const { return x; }

int16_t PlayerMovedEventDTO::getY() const { return y; }

Direction PlayerMovedEventDTO::getDirection() const { return direction; }

uint8_t PlayerMovedEventDTO::getCode() {
    return static_cast<uint8_t>(ServerOpcode::PLAYER_MOVED);
}

std::unique_ptr<CommandDTO> PlayerMovedEventDTO::clone() {
    return std::make_unique<PlayerMovedEventDTO>(*this);
}
