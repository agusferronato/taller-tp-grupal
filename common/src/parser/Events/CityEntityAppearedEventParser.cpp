#include "parser/Events/CityEntityAppearedEventParser.h"
#include "CityEntityAppearedEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"
#include <variant>

void CityEntityAppearedEventParser::serialize(std::vector<uint8_t> &bytes,
                                              const ServerEventDTO &dto) {
  const auto &event = std::get<CityEntityAppearedEventDTO>(dto);
  utils.appendBytes(static_cast<uint8_t>(EventOpcode::CityEntityAppearedEvent),
                    bytes);
  utils.appendBytes(event.entityId, bytes);
  utils.appendBytes(event.type, bytes);
  utils.appendBytes(event.x, bytes);
  utils.appendBytes(event.y, bytes);
  utils.appendBytes(static_cast<uint8_t>(event.direction), bytes);
}

ServerEventDTO CityEntityAppearedEventParser::deserialize(Protocol &protocol) {
  uint32_t entityId = protocol.getUint32();
  uint8_t type = protocol.getUint8();
  int16_t x = protocol.getInt16();
  int16_t y = protocol.getInt16();
  Direction direction = static_cast<Direction>(protocol.getUint8());
  return CityEntityAppearedEventDTO{entityId, type, x, y, direction};
}
