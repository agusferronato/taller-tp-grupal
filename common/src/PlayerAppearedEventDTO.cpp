#include "PlayerAppearedEventDTO.h"

PlayerAppearedEventDTO::PlayerAppearedEventDTO(PlayerId player_id, int16_t x, int16_t y, Direction direction)
    : player_id(player_id), x(x), y(y), direction(direction) {}

PlayerId PlayerAppearedEventDTO::getPlayerId() const { return player_id; }

int16_t PlayerAppearedEventDTO::getX() const { return x; }

int16_t PlayerAppearedEventDTO::getY() const { return y; }

Direction PlayerAppearedEventDTO::getDirection() const { return direction; }

uint8_t PlayerAppearedEventDTO::getCode() {
    return static_cast<uint8_t>(ServerOpcode::PLAYER_APPEARED);
}

std::unique_ptr<CommandDTO> PlayerAppearedEventDTO::clone() {
    return std::make_unique<PlayerAppearedEventDTO>(*this);
}
