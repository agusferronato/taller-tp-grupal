#include "parser/Events/PlayerStoppedEventParser.h"

#include "PlayerStoppedEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void PlayerStoppedEventParser::serialize(std::vector<uint8_t> &bytes,
                                         const ServerEventDTO &dto) {
  const auto &event = std::get<PlayerStoppedEventDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(EventOpcode::PlayerStoppedEvent),
                    bytes);
  utils.appendBytes(event.player_id, bytes);
}

ServerEventDTO PlayerStoppedEventParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();

  return PlayerStoppedEventDTO{playerId};
}
