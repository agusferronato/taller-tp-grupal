#ifndef PLAYER_MOVED_EVENT_DTO_H
#define PLAYER_MOVED_EVENT_DTO_H

#include "CommandDTO.h"
#include "protocol_codes.h"
#include <cstdint>

class PlayerMovedEventDTO : public CommandDTO {
private:
    uint32_t player_id;
    uint16_t x;
    uint16_t y;

public:
    PlayerMovedEventDTO(uint32_t player_id, uint16_t x, uint16_t y);

    uint32_t getPlayerId() const;
    uint16_t getX() const;
    uint16_t getY() const;

    uint8_t getCode() override;
};

#endif
