#ifndef PLAYER_STOPPED_EVENT_DTO_H
#define PLAYER_STOPPED_EVENT_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct PlayerStoppedEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::PlayerStoppedEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint32_t player_id;
};

#endif
