#include "parser/Events/GlobalChatMessageEventParser.h"

#include "DTO/Events/GlobalChatMessageEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void GlobalChatMessageEventParser::serialize(std::vector<uint8_t> &bytes,
                                               const ServerEventDTO &dto) {
  const auto &request = std::get<GlobalChatMessageEventDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(EventOpcode::GlobalChatMessageEvent), bytes);
  utils.appendBytes(request.playerName, bytes);
  utils.appendBytes(request.message, bytes);
}

ServerEventDTO GlobalChatMessageEventParser::deserialize(Protocol &protocol) {
  std::string playerName = protocol.getStringData();
  std::string message = protocol.getStringData();

  return GlobalChatMessageEventDTO{std::move(playerName), std::move(message)};
}