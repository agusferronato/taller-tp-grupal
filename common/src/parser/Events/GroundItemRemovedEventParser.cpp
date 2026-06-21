#include "parser/Events/GroundItemRemovedEventParser.h"

#include "DTO/Events/GroundItemRemovedEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void GroundItemRemovedEventParser::serialize(std::vector<uint8_t> &bytes,
                                             const ServerEventDTO &dto) {
  const auto &event = std::get<GroundItemRemovedEventDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(EventOpcode::GroundItemRemovedEvent),
                    bytes);
  utils.appendBytes(event.groundItemId, bytes);
}

ServerEventDTO GroundItemRemovedEventParser::deserialize(Protocol &protocol) {
  GroundItemRemovedEventDTO event;
  event.groundItemId = protocol.getUint32();
  return event;
}
