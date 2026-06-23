#ifndef NPC_APPEARED_EVENT_DTO_H
#define NPC_APPEARED_EVENT_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct NPCAppearedEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::NPCAppearedEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint32_t npcId;
  uint8_t npcType;
  int16_t x;
  int16_t y;
};

#endif
