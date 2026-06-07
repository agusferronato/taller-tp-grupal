#include "parser/Events/CityEntityMovedEventParser.h"
#include "CityEntityMovedEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"
#include <variant>

void CityEntityMovedEventParser::serialize(std::vector<uint8_t> &bytes,
                                            const ServerEventDTO &dto) {
  const auto &event = std::get<CityEntityMovedEventDTO>(dto);
  utils.appendBytes(static_cast<uint8_t>(EventOpcode::CityEntityMovedEvent), bytes);
  utils.appendBytes(event.entityId, bytes);
  utils.appendBytes(event.x, bytes);
  utils.appendBytes(event.y, bytes);
  utils.appendBytes(static_cast<uint8_t>(event.direction), bytes);
}

ServerEventDTO CityEntityMovedEventParser::deserialize(Protocol &protocol) {
  uint32_t entityId = protocol.getUint32();
  int16_t x = protocol.getInt16();
  int16_t y = protocol.getInt16();
  Direction direction = static_cast<Direction>(protocol.getUint8());
  return CityEntityMovedEventDTO{entityId, x, y, direction};
}
