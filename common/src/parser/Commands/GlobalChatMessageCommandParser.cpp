#include "parser/Commands/GlobalChatMessageCommandParser.h"

#include "DTO/Commands/GlobalChatMessageCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void GlobalChatMessageCommandParser::serialize(std::vector<uint8_t> &bytes,
                                               const ClientCommandDTO &dto) {
  const auto &request = std::get<GlobalChatMessageCommandDTO>(dto);

  utils.appendBytes(
      static_cast<uint8_t>(ClientCommandOpCode::GlobalChatMessageCommand),
      bytes);
  utils.appendBytes(request.playerId, bytes);
  utils.appendBytes(request.message, bytes);
}

ClientCommandDTO
GlobalChatMessageCommandParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();
  std::string message = protocol.getStringData();

  return GlobalChatMessageCommandDTO{playerId, std::move(message)};
}
