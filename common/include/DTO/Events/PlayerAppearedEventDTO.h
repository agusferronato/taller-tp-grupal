#ifndef PLAYER_APPEARED_EVENT_DTO_H
#define PLAYER_APPEARED_EVENT_DTO_H

#include <cstdint>
#include <string>

#include "Direction.h"
#include "protocol/ProtocolCodes.h"

struct PlayerAppearedEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::PlayerAppearedEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint32_t playerId;
  int16_t x;
  int16_t y;
  Direction direction;
  std::string race;
  std::string playerName;
  uint32_t hp;
  uint32_t maxHp;
  uint32_t mana;
  uint32_t maxMana;
  uint32_t gold;
  uint32_t level;
  uint32_t experience;
};

#endif // PLAYER_APPEARED_EVENT_DTO_H
