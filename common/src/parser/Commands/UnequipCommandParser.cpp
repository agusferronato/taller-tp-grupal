#include "parser/Commands/UnequipCommandParser.h"

#include "DTO/Commands/UnequipCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void UnequipCommandParser::serialize(std::vector<uint8_t> &bytes,
                                     const ClientCommandDTO &dto) {
  const auto &request = std::get<UnequipCommandDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(ClientCommandOpCode::UnequipCommand),
                    bytes);
  utils.appendBytes(request.playerId, bytes);
  utils.appendBytes(request.equipSlot, bytes);
}

ClientCommandDTO UnequipCommandParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();
  uint8_t equipSlot = protocol.getUint8();

  return UnequipCommandDTO{playerId, equipSlot};
}
