#include "parser/Events/NPCMovedEventParser.h"
#include "NPCMovedEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"
#include <variant>

void NPCMovedEventParser::serialize(std::vector<uint8_t> &bytes,
                                    const ServerEventDTO &dto) {
  const auto &event = std::get<NPCMovedEventDTO>(dto);
  utils.appendBytes(static_cast<uint8_t>(EventOpcode::NPCMovedEvent), bytes);
  utils.appendBytes(event.npcId, bytes);
  utils.appendBytes(event.x, bytes);
  utils.appendBytes(event.y, bytes);
  utils.appendBytes(static_cast<uint8_t>(event.direction), bytes);
}

ServerEventDTO NPCMovedEventParser::deserialize(Protocol &protocol) {
  uint32_t npcId = protocol.getUint32();
  int16_t x = protocol.getInt16();
  int16_t y = protocol.getInt16();
  Direction direction = static_cast<Direction>(protocol.getUint8());
  return NPCMovedEventDTO{npcId, x, y, direction};
}
