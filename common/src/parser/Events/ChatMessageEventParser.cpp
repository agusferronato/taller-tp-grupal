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
  utils.appendBytes(static_cast<uint8_t>(event.category), bytes);
  utils.appendBytes(event.sender, bytes);
  utils.appendBytes(event.message, bytes);
}

ServerEventDTO ChatMessageEventParser::deserialize(Protocol &protocol) {
  ChatMessageCategory category =
      static_cast<ChatMessageCategory>(protocol.getUint8());
  std::string sender = protocol.getStringData();
  std::string message = protocol.getStringData();

  return ChatMessageEventDTO{category, std::move(sender), std::move(message)};
}
