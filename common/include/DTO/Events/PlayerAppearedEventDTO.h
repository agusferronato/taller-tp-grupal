#ifndef PLAYER_APPEARED_EVENT_DTO_H
#define PLAYER_APPEARED_EVENT_DTO_H

#include <cstdint>

#include "Direction.h"
#include "protocol/ProtocolCodes.h"

struct PlayerAppearedEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::PlayerAppearedEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint32_t playerId;
  int16_t x;
  int16_t y;
  Direction direction;
};

#endif // PLAYER_APPEARED_EVENT_DTO_H
