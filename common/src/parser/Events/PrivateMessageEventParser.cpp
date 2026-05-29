#include "parser/Events/PrivateMessageEventParser.h"

#include "DTO/Events/PrivateMessageEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void PrivateMessageEventParser::serialize(std::vector<uint8_t> &bytes,
                                     const ServerEventDTO &dto) {
  const auto &request = std::get<PrivateMessageEventDTO>(dto);

  utils.appendBytes(
      static_cast<uint8_t>(ClientCommandOpCode::PrivateMessageCommand), bytes);

  utils.appendBytes(request.targetName, bytes);
  utils.appendBytes(request.message, bytes);
}

ServerEventDTO PrivateMessageEventParser::deserialize(Protocol &protocol) {
  std::string senderName = protocol.getStringData();
  std::string targetName = protocol.getStringData();
  std::string message = protocol.getStringData();

  return PrivateMessageEventDTO{std::move(senderName), std::move(targetName), std::move(message)};
}
