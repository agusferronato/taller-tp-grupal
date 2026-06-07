#include "parser/Events/GroundItemsListEventParser.h"

#include "GroundItemsListEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void GroundItemsListEventParser::serialize(std::vector<uint8_t> &bytes,
                                            const ServerEventDTO &dto) {
  const auto &event = std::get<GroundItemsListEventDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(EventOpcode::GroundItemsListEvent),
                    bytes);
  utils.appendBytes(static_cast<uint16_t>(event.items.size()), bytes);

  for (const auto &item : event.items) {
    utils.appendBytes(item.groundItemId, bytes);
    utils.appendBytes(item.itemId, bytes);
    utils.appendBytes(item.x, bytes);
    utils.appendBytes(item.y, bytes);
  }
}

ServerEventDTO GroundItemsListEventParser::deserialize(Protocol &protocol) {
  uint16_t count = protocol.getUint16();
  std::vector<GroundItemInfoDTO> items;
  items.reserve(count);

  for (uint16_t i = 0; i < count; ++i) {
    uint32_t id = protocol.getUint32();
    uint8_t itemId = protocol.getUint8();
    int16_t x = protocol.getInt16();
    int16_t y = protocol.getInt16();
    items.push_back(GroundItemInfoDTO{id, itemId, x, y});
  }

  return GroundItemsListEventDTO{std::move(items)};
}
