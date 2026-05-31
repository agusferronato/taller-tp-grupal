#include "parser/Commands/DropItemCommandParser.h"

#include "DTO/Commands/DropItemCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void DropItemCommandParser::serialize(std::vector<uint8_t> &bytes,
                                      const ClientCommandDTO &dto) {
  const auto &request = std::get<DropItemCommandDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(ClientCommandOpCode::DropItemCommand),
                    bytes);
  utils.appendBytes(request.playerId, bytes);
  utils.appendBytes(request.inventorySlot, bytes);
}

ClientCommandDTO DropItemCommandParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();
  uint8_t inventorySlot = protocol.getUint8();

  return DropItemCommandDTO{playerId, inventorySlot};
}
