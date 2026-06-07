#include "parser/Events/NPCAppearedEventParser.h"

#include "NPCAppearedEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void NPCAppearedEventParser::serialize(std::vector<uint8_t> &bytes,
                                       const ServerEventDTO &dto) {
  const auto &event = std::get<NPCAppearedEventDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(EventOpcode::NPCAppearedEvent), bytes);
  utils.appendBytes(event.npcId, bytes);
  utils.appendBytes(event.npcType, bytes);
  utils.appendBytes(event.x, bytes);
  utils.appendBytes(event.y, bytes);
}

ServerEventDTO NPCAppearedEventParser::deserialize(Protocol &protocol) {
  uint32_t npcId = protocol.getUint32();
  uint8_t npcType = protocol.getUint8();
  int16_t x = protocol.getInt16();
  int16_t y = protocol.getInt16();

  return NPCAppearedEventDTO{npcId, npcType, x, y};
}
