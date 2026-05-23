#include "parser/Events/RegisterPlayerResponseParser.h"

#include "RegisterPlayerResponseDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void RegisterPlayerResponseParser::serialize(std::vector<uint8_t> &bytes,
                                             const ServerEventDTO &dto) {
  const auto &event = std::get<RegisterPlayerResponseDTO>(dto);

  utils.appendToSend(static_cast<uint8_t>(ServerOpcode::RegisterResponse),
                     bytes);
  utils.appendToSend(event.playerId, bytes);
  utils.appendToSend(event.status, bytes);
}

ServerEventDTO RegisterPlayerResponseParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();
  uint8_t status = protocol.getUint8();

  return RegisterPlayerResponseDTO{playerId, status};
}
