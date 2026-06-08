#include "parser/Events/PlayerResurrectEventParser.h"

#include "PlayerResurrectEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void PlayerResurrectEventParser::serialize(std::vector<uint8_t> &bytes,
                                           const ServerEventDTO &dto) {
  const auto &event = std::get<PlayerResurrectEventDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(EventOpcode::PlayerResurrectEvent),
                    bytes);
  utils.appendBytes(event.playerId, bytes);
  utils.appendBytes(event.x, bytes);
  utils.appendBytes(event.y, bytes);
}

ServerEventDTO PlayerResurrectEventParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();
  int16_t x = protocol.getInt16();
  int16_t y = protocol.getInt16();

  return PlayerResurrectEventDTO{playerId, x, y};
}
