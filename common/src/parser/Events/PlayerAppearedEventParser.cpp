#include "parser/Events/PlayerAppearedEventParser.h"

#include "PlayerAppearedEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void PlayerAppearedEventParser::serialize(std::vector<uint8_t> &bytes,
                                          const ServerEventDTO &dto) {
  const auto &event = std::get<PlayerAppearedEventDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(EventOpcode::PlayerAppearedEvent),
                    bytes);
  utils.appendBytes(event.playerId, bytes);
  utils.appendBytes(event.x, bytes);
  utils.appendBytes(event.y, bytes);
  utils.appendBytes(static_cast<uint8_t>(event.direction), bytes);
  utils.appendBytes(event.race, bytes);
  utils.appendBytes(event.playerName, bytes);
  utils.appendBytes(event.hp, bytes);
  utils.appendBytes(event.maxHp, bytes);
  utils.appendBytes(event.mana, bytes);
  utils.appendBytes(event.maxMana, bytes);
  utils.appendBytes(event.gold, bytes);
  utils.appendBytes(event.level, bytes);
  utils.appendBytes(event.experience, bytes);
}

ServerEventDTO PlayerAppearedEventParser::deserialize(Protocol &protocol) {
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

  return PlayerAppearedEventDTO{playerId,     x,          y,
                                 direction,    std::move(race),
                                 std::move(playerName), hp,
                                 maxHp,        mana,      maxMana,
                                 gold,         level,     experience};
}
