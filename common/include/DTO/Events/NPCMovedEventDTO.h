#ifndef NPC_MOVED_EVENT_DTO_H
#define NPC_MOVED_EVENT_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

#include "Direction.h"

struct NPCMovedEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::NPCMovedEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint32_t npcId;
  int16_t x;
  int16_t y;
  Direction direction;
};

#endif
