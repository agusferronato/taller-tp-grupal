#include "parser/Events/PlayerAppearedEventParser.h"

#include "PlayerAppearedEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void PlayerAppearedEventParser::serialize(std::vector<uint8_t> &bytes,
                                          const ServerEventDTO &dto) {
  const auto &event = std::get<PlayerAppearedEventDTO>(dto);

  utils.appendToSend(static_cast<uint8_t>(EventOpcode::PlayerAppearedEvent),
                     bytes);
  utils.appendToSend(event.playerId, bytes);
  utils.appendToSend(event.x, bytes);
  utils.appendToSend(event.y, bytes);
  utils.appendToSend(static_cast<uint8_t>(event.direction), bytes);
}

ServerEventDTO PlayerAppearedEventParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();
  int16_t x = protocol.getInt16();
  int16_t y = protocol.getInt16();
  Direction direction = static_cast<Direction>(protocol.getUint8());

  return PlayerAppearedEventDTO{playerId, x, y, direction};
}
