#include "MoveCommandDTO.h"

MoveCommandDTO::MoveCommandDTO(uint32_t player_id, Direction direction)
    : player_id(player_id), direction(direction) {}

uint32_t MoveCommandDTO::getPlayerId() const { return player_id; }

Direction MoveCommandDTO::getDirection() const { return direction; }

uint8_t MoveCommandDTO::getCode() {
  return static_cast<uint8_t>(CommandOpCode::MoveCommand);
}

std::unique_ptr<CommandDTO> MoveCommandDTO::clone() {
  return std::make_unique<MoveCommandDTO>(*this);
}
