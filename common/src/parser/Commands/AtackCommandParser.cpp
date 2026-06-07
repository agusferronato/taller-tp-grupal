#include "parser/Commands/AtackCommandParser.h"

#include "DTO/Commands/AtackCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void AtackCommandParser::serialize(std::vector<uint8_t> &bytes,
                                   const ClientCommandDTO &dto) {
  const auto &request = std::get<AtackCommandDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(ClientCommandOpCode::AtackCommand),
                    bytes);

  utils.appendBytes(request.playerId, bytes);

  utils.appendBytes(request.x, bytes);

  utils.appendBytes(request.y, bytes);
}

ClientCommandDTO AtackCommandParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();
  int16_t x = protocol.getInt16();
  int16_t y = protocol.getInt16();

  return AtackCommandDTO{playerId, x, y};
}
