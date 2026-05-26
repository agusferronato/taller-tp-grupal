#ifndef NPC_DEFEATED_EVENT_DTO_H
#define NPC_DEFEATED_EVENT_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct NpcDefeatedEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::NPCDefeatedEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint8_t npcId;
};

#endif // NPC_DEFEATED_EVENT_DTO_H
