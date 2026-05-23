#include "parser/Commands/PlayerStopCommandParser.h"

#include "PlayerStopDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void PlayerStopCommandParser::serialize(std::vector<uint8_t> &bytes,
                                        const ClientRequestDTO &dto) {
  const auto &event = std::get<PlayerStopDTO>(dto);

  utils.appendToSend(static_cast<uint8_t>(CommandOpCode::PlayerStop), bytes);
  utils.appendToSend(event.player_id, bytes);
}

ClientRequestDTO PlayerStopCommandParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();

  return PlayerStopDTO{playerId};
}
