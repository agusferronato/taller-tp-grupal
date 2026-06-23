#include "parser/Events/GroundItemAppearedEventParser.h"

#include "DTO/Events/GroundItemAppearedEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void GroundItemAppearedEventParser::serialize(std::vector<uint8_t> &bytes,
                                              const ServerEventDTO &dto) {
  const auto &event = std::get<GroundItemAppearedEventDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(EventOpcode::GroundItemAppearedEvent),
                    bytes);
  utils.appendBytes(event.groundItemId, bytes);
  utils.appendBytes(event.itemId, bytes);
  utils.appendBytes(event.x, bytes);
  utils.appendBytes(event.y, bytes);
}

ServerEventDTO GroundItemAppearedEventParser::deserialize(Protocol &protocol) {
  GroundItemAppearedEventDTO event;
  event.groundItemId = protocol.getUint32();
  event.itemId = protocol.getUint8();
  event.x = protocol.getInt16();
  event.y = protocol.getInt16();
  return event;
}
