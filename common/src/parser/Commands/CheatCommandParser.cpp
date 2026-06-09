#include "parser/Commands/CheatCommandParser.h"

#include "DTO/Commands/CheatCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void CheatCommandParser::serialize(std::vector<uint8_t> &bytes,
                                   const ClientCommandDTO &dto) {
  const auto &request = std::get<CheatCommandDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(ClientCommandOpCode::CheatCommand),
                    bytes);
  utils.appendBytes(static_cast<uint8_t>(request.cheat), bytes);
}

ClientCommandDTO CheatCommandParser::deserialize(Protocol &protocol) {
  uint8_t cheat = protocol.getUint8();

  return CheatCommandDTO{static_cast<CheatType>(cheat)};
}
