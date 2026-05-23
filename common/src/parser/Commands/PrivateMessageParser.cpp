#include "parser/Commands/PrivateMessageParser.h"

#include "DTO/Commands/PrivateMessageDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void PrivateMessageParser::serialize(std::vector<uint8_t> &bytes,
                                     const ClientRequestDTO &dto) {
  const auto &request = std::get<PrivateMessageDTO>(dto);

  utils.appendToSend(static_cast<uint8_t>(CommandOpCode::PrivateMessage),
                     bytes);

  utils.appendToSend(request.target, bytes);
  utils.appendToSend(request.message, bytes);
}

ClientRequestDTO PrivateMessageParser::deserialize(Protocol &protocol) {
  std::string target = protocol.getStringData();
  std::string message = protocol.getStringData();

  return PrivateMessageDTO{std::move(target), std::move(message)};
}
