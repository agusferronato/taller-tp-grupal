#include "parser/Events/NPCStoppedEventParser.h"
#include "NPCStoppedEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"
#include <variant>

void NPCStoppedEventParser::serialize(std::vector<uint8_t> &bytes,
                                      const ServerEventDTO &dto) {
  const auto &event = std::get<NPCStoppedEventDTO>(dto);
  utils.appendBytes(static_cast<uint8_t>(EventOpcode::NPCStoppedEvent), bytes);
  utils.appendBytes(event.npcId, bytes);
}

ServerEventDTO NPCStoppedEventParser::deserialize(Protocol &protocol) {
  uint32_t npcId = protocol.getUint32();
  return NPCStoppedEventDTO{npcId};
}
