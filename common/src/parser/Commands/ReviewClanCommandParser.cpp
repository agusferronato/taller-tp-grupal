#include "parser/Commands/ReviewClanCommandParser.h"

#include "DTO/Commands/ReviewClanCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void ReviewClanCommandParser::serialize(std::vector<uint8_t> &bytes,
                                        const ClientCommandDTO &dto) {
  const auto &request = std::get<ReviewClanCommandDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(ClientCommandOpCode::ReviewClanCommand),
                    bytes);
  utils.appendBytes(request.playerId, bytes);
}

ClientCommandDTO ReviewClanCommandParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();

  return ReviewClanCommandDTO{playerId};
}
