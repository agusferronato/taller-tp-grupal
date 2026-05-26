#ifndef PLAYER_REMOVED_EVENT_DTO_H
#define PLAYER_REMOVED_EVENT_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct PlayerRemovedEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::PlayerRemovedEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint32_t playerId;
};

#endif
