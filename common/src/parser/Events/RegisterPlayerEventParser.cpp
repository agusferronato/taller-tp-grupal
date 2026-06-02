#include "parser/Events/RegisterPlayerEventParser.h"

#include "RegisterPlayerEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void RegisterPlayerEventParser::serialize(std::vector<uint8_t> &bytes,
                                          const ServerEventDTO &dto) {
  const auto &event = std::get<RegisterPlayerEventDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(EventOpcode::RegisterPlayerEvent),
                    bytes);
  utils.appendBytes(event.playerId, bytes);
  utils.appendBytes(event.status, bytes);
}

ServerEventDTO RegisterPlayerEventParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();
  uint8_t status = protocol.getUint8();

  return RegisterPlayerEventDTO{playerId, status};
}
