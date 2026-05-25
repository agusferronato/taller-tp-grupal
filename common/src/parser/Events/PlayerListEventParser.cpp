#include "parser/Events/PlayerListEventParser.h"

#include "PlayerListEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void PlayerListEventParser::serialize(std::vector<uint8_t> &bytes,
                                      const ServerEventDTO &dto) {
  const auto &event = std::get<PlayerListEventDTO>(dto);

  utils.appendToSend(static_cast<uint8_t>(EventOpcode::PlayerListEvent), bytes);
  utils.appendToSend(static_cast<uint16_t>(event.players.size()), bytes);

  for (const auto &player : event.players) {
    utils.appendToSend(player.player_id, bytes);
    utils.appendToSend(player.x, bytes);
    utils.appendToSend(player.y, bytes);
    utils.appendToSend(static_cast<uint8_t>(player.direction), bytes);
  }
}

ServerEventDTO PlayerListEventParser::deserialize(Protocol &protocol) {
  uint16_t playerCount = protocol.getUint16();
  std::vector<PlayerInfoDTO> players;
  players.reserve(playerCount);

  for (uint16_t i = 0; i < playerCount; ++i) {
    uint32_t playerId = protocol.getUint32();
    int16_t x = protocol.getInt16();
    int16_t y = protocol.getInt16();
    Direction direction = static_cast<Direction>(protocol.getUint8());

    players.push_back(PlayerInfoDTO{playerId, x, y, direction});
  }

  return PlayerListEventDTO{std::move(players)};
}
