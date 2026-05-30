#ifndef PLAYER_INFO_EVENT_DTO_H
#define PLAYER_INFO_EVENT_DTO_H

#include <cstdint>

#include "protocol/ProtocolCodes.h"

struct PlayerInfoEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::PlayerInfoEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint32_t playerId;
  uint32_t hp;
  uint32_t maxHp;
  uint32_t mana;
  uint32_t maxMana;
  uint32_t gold;
  uint32_t level;
  uint32_t experience;
};

#endif
