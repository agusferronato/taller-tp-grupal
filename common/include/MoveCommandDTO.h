#ifndef MOVE_COMMAND_DTO_H
#define MOVE_COMMAND_DTO_H

#include "CommandDTO.h"
#include "direction.h"
#include "protocol_codes.h"

class MoveCommandDTO : public CommandDTO {
private:
    Direction direction;

public:
    explicit MoveCommandDTO(Direction direction) : direction(direction) {}

    Direction getDirection() const { return direction; }

    uint8_t getCode() override {
        return static_cast<uint8_t>(CommandOpCode::MOVE_COMMAND);
    }
};

#endif
