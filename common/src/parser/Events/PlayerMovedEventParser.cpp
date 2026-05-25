#include "parser/Events/PlayerMovedEventParser.h"

#include "PlayerMovedEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void PlayerMovedEventParser::serialize(std::vector<uint8_t> &bytes,
                                       const ServerEventDTO &dto) {
  const auto &event = std::get<PlayerMovedEventDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(EventOpcode::PlayerMovedEvent), bytes);
  utils.appendBytes(event.playerId, bytes);
  utils.appendBytes(event.x, bytes);
  utils.appendBytes(event.y, bytes);
  utils.appendBytes(static_cast<uint8_t>(event.direction), bytes);
}

ServerEventDTO PlayerMovedEventParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();
  int16_t x = protocol.getInt16();
  int16_t y = protocol.getInt16();
  Direction direction = static_cast<Direction>(protocol.getUint8());

  return PlayerMovedEventDTO{playerId, x, y, direction};
}
