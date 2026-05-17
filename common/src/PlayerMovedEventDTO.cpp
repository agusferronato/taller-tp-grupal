#include "PlayerMovedEventDTO.h"

PlayerMovedEventDTO::PlayerMovedEventDTO(uint32_t player_id, uint16_t x, uint16_t y)
    : player_id(player_id), x(x), y(y) {}

uint32_t PlayerMovedEventDTO::getPlayerId() const { return player_id; }

uint16_t PlayerMovedEventDTO::getX() const { return x; }

uint16_t PlayerMovedEventDTO::getY() const { return y; }

uint8_t PlayerMovedEventDTO::getCode() {
    return static_cast<uint8_t>(ServerOpcode::PLAYER_MOVED);
}
