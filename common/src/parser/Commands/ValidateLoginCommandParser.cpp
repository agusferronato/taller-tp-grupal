#include "parser/Commands/ValidateLoginCommandParser.h"

#include "DTO/Commands/ValidateLoginCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void ValidateLoginCommandParser::serialize(std::vector<uint8_t> &bytes,
                                           const ClientCommandDTO &dto) {
  const auto &request = std::get<ValidateLoginCommandDTO>(dto);

  utils.appendBytes(
      static_cast<uint8_t>(ClientCommandOpCode::ValidateLoginCommand), bytes);
  utils.appendBytes(request.playerName, bytes);
}

ClientCommandDTO ValidateLoginCommandParser::deserialize(Protocol &protocol) {
  std::string playerName = protocol.getStringData();

  return ValidateLoginCommandDTO{std::move(playerName)};
}
