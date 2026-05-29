#include "parser/Commands/PrivateMessageParser.h"

#include "DTO/Commands/PrivateMessageCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void PrivateMessageParser::serialize(std::vector<uint8_t> &bytes,
                                     const ClientCommandDTO &dto) {
  const auto &request = std::get<PrivateMessageCommandDTO>(dto);

  utils.appendBytes(
      static_cast<uint8_t>(ClientCommandOpCode::PrivateMessageCommand), bytes);

  utils.appendBytes(request.targetName, bytes);
  utils.appendBytes(request.message, bytes);
}

ClientCommandDTO PrivateMessageParser::deserialize(Protocol &protocol) {
  std::string targetName = protocol.getStringData();
  std::string message = protocol.getStringData();

  return PrivateMessageCommandDTO{std::move(targetName), std::move(message)};
}
