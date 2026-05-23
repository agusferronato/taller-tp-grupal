#include "parser/Commands/RegisterPlayerParser.h"

#include "DTO/Commands/RegisterPlayerDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void RegisterPlayerParser::serialize(std::vector<uint8_t> &bytes,
                                     const ClientRequestDTO &dto) {
  const auto &request = std::get<RegisterPlayerDTO>(dto);

  utils.appendToSend(static_cast<uint8_t>(CommandOpCode::RegisterPlayer),
                     bytes);
  utils.appendToSend(request.name, bytes);
}

ClientRequestDTO RegisterPlayerParser::deserialize(Protocol &protocol) {
  std::string name = protocol.getStringData();

  return RegisterPlayerDTO{std::move(name)};
}
