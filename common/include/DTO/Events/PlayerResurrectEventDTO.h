#ifndef PLAYER_RESURRECT_EVENT_DTO_H
#define PLAYER_RESURRECT_EVENT_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct PlayerResurrectEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::PlayerResurrectEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint32_t playerId;
  int16_t x;
  int16_t y;
};

#endif
