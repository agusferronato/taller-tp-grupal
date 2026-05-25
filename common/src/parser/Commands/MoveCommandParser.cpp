#include "parser/Commands/MoveCommandParser.h"

#include "DTO/Commands/MoveCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void MoveCommandParser::serialize(std::vector<uint8_t> &bytes,
                                  const ClientCommandDTO &dto) {
  const auto &request = std::get<MoveCommandDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(ClientCommandOpCode::MoveCommand),
                    bytes);

  utils.appendBytes(request.playerId, bytes);

  utils.appendBytes(static_cast<uint8_t>(request.direction), bytes);
}

ClientCommandDTO MoveCommandParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();
  uint8_t directionRaw = protocol.getUint8();

  return MoveCommandDTO{playerId, static_cast<Direction>(directionRaw)};
}
