#include "parser/Commands/ExitParser.h"

#include "DTO/Commands/ExitDTO.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void ExitParser::serialize(std::vector<uint8_t> &bytes,
                           const ClientRequestDTO &dto) {
  std::get<ExitDTO>(dto);

  utils.appendToSend(static_cast<uint8_t>(CommandOpCode::Exit), bytes);
}

ClientRequestDTO ExitParser::deserialize(Protocol &) { return ExitDTO{}; }
