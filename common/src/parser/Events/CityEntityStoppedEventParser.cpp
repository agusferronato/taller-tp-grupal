#include "parser/Events/CityEntityStoppedEventParser.h"
#include "CityEntityStoppedEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"
#include <variant>

void CityEntityStoppedEventParser::serialize(std::vector<uint8_t> &bytes,
                                              const ServerEventDTO &dto) {
  const auto &event = std::get<CityEntityStoppedEventDTO>(dto);
  utils.appendBytes(static_cast<uint8_t>(EventOpcode::CityEntityStoppedEvent), bytes);
  utils.appendBytes(event.entityId, bytes);
}

ServerEventDTO CityEntityStoppedEventParser::deserialize(Protocol &protocol) {
  uint32_t entityId = protocol.getUint32();
  return CityEntityStoppedEventDTO{entityId};
}
