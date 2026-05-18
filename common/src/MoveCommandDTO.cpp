#include "MoveCommandDTO.h"

MoveCommandDTO::MoveCommandDTO(PlayerId player_id, Direction direction)
    : player_id(player_id), direction(direction) {}

PlayerId MoveCommandDTO::getPlayerId() const { return player_id; }

Direction MoveCommandDTO::getDirection() const { return direction; }

uint8_t MoveCommandDTO::getCode() {
    return static_cast<uint8_t>(CommandOpCode::MOVE_COMMAND);
}

std::unique_ptr<CommandDTO> MoveCommandDTO::clone() {
    return std::make_unique<MoveCommandDTO>(*this);
}
