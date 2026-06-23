#include "parser/Commands/EquipCommandParser.h"

#include "DTO/Commands/EquipCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void EquipCommandParser::serialize(std::vector<uint8_t> &bytes,
                                   const ClientCommandDTO &dto) {
  const auto &request = std::get<EquipCommandDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(ClientCommandOpCode::EquipCommand),
                    bytes);
  utils.appendBytes(request.playerId, bytes);
  utils.appendBytes(request.inventorySlot, bytes);
}

ClientCommandDTO EquipCommandParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();
  uint8_t inventorySlot = protocol.getUint8();

  return EquipCommandDTO{playerId, inventorySlot};
}
