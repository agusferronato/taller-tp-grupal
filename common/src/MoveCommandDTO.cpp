#include "MoveCommandDTO.h"

MoveCommandDTO::MoveCommandDTO(Direction direction) : direction(direction) {}

Direction MoveCommandDTO::getDirection() const { return direction; }

uint8_t MoveCommandDTO::getCode() {
    return static_cast<uint8_t>(CommandOpCode::MOVE_COMMAND);
}
