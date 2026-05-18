#ifndef PLAYER_MOVED_EVENT_DTO_H
#define PLAYER_MOVED_EVENT_DTO_H

#include "CommandDTO.h"
#include "direction.h"
#include "protocol_codes.h"
#include "types.h"
#include <cstdint>

class PlayerMovedEventDTO : public CommandDTO {
private:
    PlayerId player_id;
    int16_t x;
    int16_t y;
    Direction direction;

public:
    PlayerMovedEventDTO(PlayerId player_id, int16_t x, int16_t y, Direction direction);

    PlayerId getPlayerId() const;
    int16_t getX() const;
    int16_t getY() const;
    Direction getDirection() const;

    uint8_t getCode() override;
    std::unique_ptr<CommandDTO> clone() override;
};

#endif
