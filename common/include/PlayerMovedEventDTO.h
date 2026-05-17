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
    PlayerMovedEventDTO(uint32_t player_id, uint16_t x, uint16_t y)
        : player_id(player_id), x(x), y(y) {}

    uint32_t getPlayerId() const { return player_id; }
    uint16_t getX() const { return x; }
    uint16_t getY() const { return y; }

    uint8_t getCode() override {
        return static_cast<uint8_t>(ServerOpcode::PLAYER_MOVED);
    }
};

#endif
