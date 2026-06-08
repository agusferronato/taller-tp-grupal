#include "parser/Events/PlayerDieEventParser.h"

#include "PlayerDieEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void PlayerDieEventParser::serialize(std::vector<uint8_t> &bytes,
                                     const ServerEventDTO &dto) {
  const auto &event = std::get<PlayerDieEventDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(EventOpcode::PlayerDieEvent), bytes);
  utils.appendBytes(event.playerId, bytes);
}

ServerEventDTO PlayerDieEventParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();

  return PlayerDieEventDTO{playerId};
}
