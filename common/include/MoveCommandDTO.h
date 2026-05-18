#ifndef MOVE_COMMAND_DTO_H
#define MOVE_COMMAND_DTO_H

#include "CommandDTO.h"
#include "direction.h"
#include "protocol_codes.h"
#include "types.h"

class MoveCommandDTO : public CommandDTO {
private:
    PlayerId player_id;
    Direction direction;

public:
    MoveCommandDTO(PlayerId player_id, Direction direction);

    PlayerId getPlayerId() const;
    Direction getDirection() const;

    uint8_t getCode() override;
    std::unique_ptr<CommandDTO> clone() override;
};

#endif
