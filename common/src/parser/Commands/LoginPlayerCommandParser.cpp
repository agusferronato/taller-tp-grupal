#include "parser/Commands/LoginPlayerParser.h"

#include "DTO/Commands/LoginPlayerCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void LoginPlayerParser::serialize(std::vector<uint8_t> &bytes,
                                  const ClientCommandDTO &dto) {
  const auto &request = std::get<LoginPlayerCommandDTO>(dto);

  utils.appendBytes(
      static_cast<uint8_t>(ClientCommandOpCode::LoginPlayerCommand), bytes);

  utils.appendBytes(request.name, bytes);
}

ClientCommandDTO LoginPlayerParser::deserialize(Protocol &protocol) {
  std::string username = protocol.getStringData();

  return LoginPlayerCommandDTO{std::move(username)};
}
