#include "parser/Commands/LoginPlayerParser.h"

#include "DTO/Commands/LoginPlayerDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void LoginPlayerParser::serialize(std::vector<uint8_t> &bytes,
                                  const ClientRequestDTO &dto) {
  const auto &request = std::get<LoginPlayerDTO>(dto);

  utils.appendToSend(static_cast<uint8_t>(CommandOpCode::LoginPlayer), bytes);

  utils.appendToSend(request.name, bytes);
}

ClientRequestDTO LoginPlayerParser::deserialize(Protocol &protocol) {
  std::string username = protocol.getStringData();

  return LoginPlayerDTO{std::move(username)};
}
