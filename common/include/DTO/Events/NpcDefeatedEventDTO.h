#ifndef NPC_DEFEATED_EVENT_DTO_H
#define NPC_DEFEATED_EVENT_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct NpcDefeatedEventDTO {
private:
  static constexpr ServerOpcode opCode = ServerOpcode::NPCDefeated;

public:
  ServerOpcode getCode() const { return opCode; }
  uint8_t npcId;
};

#endif // NPC_DEFEATED_EVENT_DTO_H
