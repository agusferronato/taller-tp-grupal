#include "parser/Events/PlayerRemovedEventParser.h"

#include "PlayerRemovedEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void PlayerRemovedEventParser::serialize(std::vector<uint8_t> &bytes,
                                         const ServerEventDTO &dto) {
  const auto &event = std::get<PlayerRemovedEventDTO>(dto);

  utils.appendToSend(static_cast<uint8_t>(ServerOpcode::PlayerRemoved), bytes);
  utils.appendToSend(event.playerId, bytes);
}

ServerEventDTO PlayerRemovedEventParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();

  return PlayerRemovedEventDTO{playerId};
}
