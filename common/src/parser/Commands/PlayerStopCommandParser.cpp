#include "parser/Commands/PlayerStopCommandParser.h"

#include "DTO/Commands/PlayerStopCommandDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void PlayerStopCommandParser::serialize(std::vector<uint8_t> &bytes,
                                        const ClientCommandDTO &dto) {
  const auto &event = std::get<PlayerStopCommandDTO>(dto);

  utils.appendBytes(
      static_cast<uint8_t>(ClientCommandOpCode::PlayerStopCommand), bytes);
  utils.appendBytes(event.player_id, bytes);
}

ClientCommandDTO PlayerStopCommandParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();

  return PlayerStopCommandDTO{playerId};
}
