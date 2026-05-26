#include "parser/Commands/ExitParser.h"

#include "DTO/Commands/ExitCommandDTO.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void ExitParser::serialize(std::vector<uint8_t> &bytes,
                           const ClientCommandDTO &dto) {
  std::get<ExitCommandDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(ClientCommandOpCode::ExitCommand),
                    bytes);
}

ClientCommandDTO ExitParser::deserialize(Protocol &) {
  return ExitCommandDTO{};
}
