#include "parser/Events/PlayerStoppedParser.h"

#include "PlayerStoppedDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void PlayerStoppedEventParser::serialize(std::vector<uint8_t> &bytes,
                                         const ServerEventDTO &dto) {
  const auto &event = std::get<PlayerStoppedDTO>(dto);

  utils.appendToSend(static_cast<uint8_t>(ServerOpcode::PlayerStopped), bytes);
  utils.appendToSend(event.player_id, bytes);
}

ServerEventDTO PlayerStoppedEventParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();

  return PlayerStoppedDTO{playerId};
}
