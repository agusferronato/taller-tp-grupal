#include "parser/Events/LoginResultEventParser.h"

#include "DTO/Events/LoginResultEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void LoginResultEventParser::serialize(std::vector<uint8_t> &bytes,
                                       const ServerEventDTO &dto) {
  const auto &event = std::get<LoginResultEventDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(EventOpcode::LoginResultEvent), bytes);
  utils.appendBytes(event.playerId, bytes);
  utils.appendBytes(static_cast<uint8_t>(event.status), bytes);
}

ServerEventDTO LoginResultEventParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();
  LoginStatus status = static_cast<LoginStatus>(protocol.getUint8());

  return LoginResultEventDTO{playerId, status};
}
