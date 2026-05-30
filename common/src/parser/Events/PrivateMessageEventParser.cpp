#include "parser/Events/PrivateMessageEventParser.h"

#include "DTO/Events/PrivateMessageEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void PrivateMessageEventParser::serialize(std::vector<uint8_t> &bytes,
                                     const ServerEventDTO &dto) {
  const auto &event = std::get<PrivateMessageEventDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(EventOpcode::PrivateMessageEvent), bytes);
  utils.appendBytes(event.senderName, bytes);
  utils.appendBytes(event.targetName, bytes);
  utils.appendBytes(event.message, bytes);
}

ServerEventDTO PrivateMessageEventParser::deserialize(Protocol &protocol) {

  std::string senderName = protocol.getStringData();
  std::string targetName = protocol.getStringData();
  std::string message = protocol.getStringData();

  return PrivateMessageEventDTO{std::move(senderName), std::move(targetName), std::move(message)};
}
