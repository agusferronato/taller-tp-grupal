#include "parser/Commands/LeaveClanCommandParser.h"

#include "DTO/Commands/LeaveClanCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void LeaveClanCommandParser::serialize(std::vector<uint8_t> &bytes,
                                       const ClientCommandDTO &dto) {
  const auto &request = std::get<LeaveClanCommandDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(ClientCommandOpCode::LeaveClanCommand),
                    bytes);
  utils.appendBytes(request.playerId, bytes);
}

ClientCommandDTO LeaveClanCommandParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();

  return LeaveClanCommandDTO{playerId};
}
