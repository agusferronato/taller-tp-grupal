#include "parser/Events/PlayerListEventParser.h"

#include "PlayerListEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void PlayerListEventParser::serialize(std::vector<uint8_t> &bytes,
                                      const ServerEventDTO &dto) {
  const auto &event = std::get<PlayerListEventDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(EventOpcode::PlayerListEvent), bytes);
  utils.appendBytes(static_cast<uint16_t>(event.players.size()), bytes);

  for (const auto &player : event.players) {
    utils.appendBytes(player.playerId, bytes);
    utils.appendBytes(player.x, bytes);
    utils.appendBytes(player.y, bytes);
    utils.appendBytes(static_cast<uint8_t>(player.direction), bytes);
    utils.appendBytes(player.race, bytes);
    utils.appendBytes(player.playerName, bytes);
    utils.appendBytes(player.hp, bytes);
    utils.appendBytes(player.maxHp, bytes);
    utils.appendBytes(player.mana, bytes);
    utils.appendBytes(player.maxMana, bytes);
    utils.appendBytes(player.gold, bytes);
    utils.appendBytes(player.level, bytes);
    utils.appendBytes(player.experience, bytes);
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

    std::string race = protocol.getStringData();
    std::string playerName = protocol.getStringData();
    uint32_t hp = protocol.getUint32();
    uint32_t maxHp = protocol.getUint32();
    uint32_t mana = protocol.getUint32();
    uint32_t maxMana = protocol.getUint32();
    uint32_t gold = protocol.getUint32();
    uint32_t level = protocol.getUint32();
    uint32_t experience = protocol.getUint32();

    players.push_back(PlayerInfoDTO{playerId,     x,          y,
                                    direction,    std::move(race),
                                    std::move(playerName), hp,
                                    maxHp,        mana,       maxMana,
                                    gold,         level,      experience});
  }

  return PlayerListEventDTO{std::move(players)};
}
