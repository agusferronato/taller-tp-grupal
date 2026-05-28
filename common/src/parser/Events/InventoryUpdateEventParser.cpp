#include "parser/Events/InventoryUpdateEventParser.h"

#include "DTO/Events/InventoryUpdateEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void InventoryUpdateEventParser::serialize(std::vector<uint8_t> &bytes,
                                           const ServerEventDTO &dto) {
  const auto &event = std::get<InventoryUpdateEventDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(EventOpcode::InventoryUpdateEvent),
                    bytes);
  utils.appendBytes(event.playerId, bytes);
  for (auto item : event.items)
    utils.appendBytes(item, bytes);
  utils.appendBytes(event.equippedWeapon, bytes);
  utils.appendBytes(event.equippedArmor, bytes);
  utils.appendBytes(event.equippedHelmet, bytes);
  utils.appendBytes(event.equippedShield, bytes);
}

ServerEventDTO InventoryUpdateEventParser::deserialize(Protocol &protocol) {
  InventoryUpdateEventDTO event;
  event.playerId = protocol.getUint32();
  for (auto &item : event.items)
    item = protocol.getUint8();
  event.equippedWeapon = protocol.getUint8();
  event.equippedArmor = protocol.getUint8();
  event.equippedHelmet = protocol.getUint8();
  event.equippedShield = protocol.getUint8();

  return event;
}
