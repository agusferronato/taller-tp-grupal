#include "parser/Commands/TakeItemCommandParser.h"

#include "DTO/Commands/TakeItemCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void TakeItemCommandParser::serialize(std::vector<uint8_t> &bytes,
                                      const ClientCommandDTO &dto) {
  const auto &request = std::get<TakeItemCommandDTO>(dto);

  utils.appendBytes(
      static_cast<uint8_t>(ClientCommandOpCode::TakeItemCommand), bytes);
  utils.appendBytes(request.playerId, bytes);
}

ClientCommandDTO TakeItemCommandParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();

  return TakeItemCommandDTO{playerId};
}
