#ifndef PLAYER_DIE_EVENT_DTO_H
#define PLAYER_DIE_EVENT_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct PlayerDieEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::PlayerDieEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint32_t playerId;
};

#endif
