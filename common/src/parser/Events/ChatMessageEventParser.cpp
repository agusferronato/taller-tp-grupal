#include "parser/Events/ChatMessageEventParser.h"

#include "ChatMessageEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <string>
#include <variant>

void ChatMessageEventParser::serialize(std::vector<uint8_t> &bytes,
                                       const ServerEventDTO &dto) {
  const auto &event = std::get<ChatMessageEventDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(EventOpcode::ChatMessageEvent), bytes);
  utils.appendBytes(event.sender, bytes);
  utils.appendBytes(event.message, bytes);
}

ServerEventDTO ChatMessageEventParser::deserialize(Protocol &protocol) {
  std::string sender = protocol.getStringData();
  std::string message = protocol.getStringData();

  return ChatMessageEventDTO{std::move(sender), std::move(message)};
}
