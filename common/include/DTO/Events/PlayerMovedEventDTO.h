#ifndef PLAYER_MOVED_EVENT_DTO_H
#define PLAYER_MOVED_EVENT_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

#include "Direction.h"

struct PlayerMovedEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::PlayerMovedEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint32_t playerId;
  int16_t x;
  int16_t y;
  Direction direction;
};

#endif
