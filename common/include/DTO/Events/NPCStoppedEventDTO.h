#ifndef NPC_STOPPED_EVENT_DTO_H
#define NPC_STOPPED_EVENT_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct NPCStoppedEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::NPCStoppedEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint32_t npcId;
};

#endif
