#include "parser/Events/NpcDefeatedEventParser.h"

#include "NpcDefeatedEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void NpcDefeatedEventParser::serialize(std::vector<uint8_t> &bytes,
                                       const ServerEventDTO &dto) {
  const auto &event = std::get<NpcDefeatedEventDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(EventOpcode::NPCDefeatedEvent), bytes);
  utils.appendBytes(event.npcId, bytes);
}

ServerEventDTO NpcDefeatedEventParser::deserialize(Protocol &protocol) {
  uint8_t npcId = protocol.getUint8();

  return NpcDefeatedEventDTO{npcId};
}
