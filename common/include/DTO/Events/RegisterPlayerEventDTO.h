#ifndef REGISTER_PLAYER_EVENT_DTO_H
#define REGISTER_PLAYER_EVENT_DTO_H

#include "Race.h"
#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct RegisterPlayerEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::RegisterPlayerEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint32_t playerId;
  uint8_t status;
  Race race;
};

#endif // REGISTER_PLAYER_EVENT_DTO_H
