#include "parser/Commands/RegisterPlayerParser.h"

#include "DTO/Commands/RegisterPlayerCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void RegisterPlayerParser::serialize(std::vector<uint8_t> &bytes,
                                     const ClientCommandDTO &dto) {
  const auto &request = std::get<RegisterPlayerCommandDTO>(dto);

  utils.appendBytes(
      static_cast<uint8_t>(ClientCommandOpCode::RegisterPlayerCommand), bytes);
  utils.appendBytes(request.name, bytes);
}

ClientCommandDTO RegisterPlayerParser::deserialize(Protocol &protocol) {
  std::string name = protocol.getStringData();

  return RegisterPlayerCommandDTO{std::move(name)};
}
