#include "parser/Events/PlayerInfoEventParser.h"

#include "PlayerInfoEventDTO.h"
#include "protocol/Protocol.h"
#include "protocol/ProtocolCodes.h"

#include <variant>

void PlayerInfoEventParser::serialize(std::vector<uint8_t> &bytes,
                                      const ServerEventDTO &dto) {
  const auto &event = std::get<PlayerInfoEventDTO>(dto);

  utils.appendBytes(static_cast<uint8_t>(EventOpcode::PlayerInfoEvent), bytes);
  utils.appendBytes(event.playerId, bytes);
  utils.appendBytes(event.hp, bytes);
  utils.appendBytes(event.maxHp, bytes);
  utils.appendBytes(event.mana, bytes);
  utils.appendBytes(event.maxMana, bytes);
  utils.appendBytes(event.gold, bytes);
  utils.appendBytes(event.level, bytes);
  utils.appendBytes(event.experience, bytes);
}

ServerEventDTO PlayerInfoEventParser::deserialize(Protocol &protocol) {
  uint32_t playerId = protocol.getUint32();
  uint32_t hp = protocol.getUint32();
  uint32_t maxHp = protocol.getUint32();
  uint32_t mana = protocol.getUint32();
  uint32_t maxMana = protocol.getUint32();
  uint32_t gold = protocol.getUint32();
  uint32_t level = protocol.getUint32();
  uint32_t experience = protocol.getUint32();

  return PlayerInfoEventDTO{playerId, hp,   maxHp, mana,
                            maxMana,  gold, level, experience};
}
