#ifndef MOVE_COMMAND_DTO_H
#define MOVE_COMMAND_DTO_H

#include "CommandDTO.h"
#include "direction.h"
#include "protocol_codes.h"

class MoveCommandDTO : public CommandDTO {
private:
    Direction direction;

public:
    explicit MoveCommandDTO(Direction direction);

    Direction getDirection() const;

    uint8_t getCode() override;
};

#endif
