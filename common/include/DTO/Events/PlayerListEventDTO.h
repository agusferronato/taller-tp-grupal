#ifndef PLAYER_LIST_DTO_H
#define PLAYER_LIST_DTO_H

#include "Direction.h"
#include "PlayerClass.h"
#include "Race.h"
#include "protocol/ProtocolCodes.h"
#include <cstdint>
#include <string>
#include <vector>

// Struct auxiliar
struct PlayerInfoDTO {
  uint32_t playerId;
  int16_t x;
  int16_t y;
  Direction direction;
  Race race;
  PlayerClass playerClass;
  std::string playerName;
  uint32_t hp;
  uint32_t maxHp;
  uint32_t mana;
  uint32_t maxMana;
  uint32_t gold;
  uint32_t level;
  uint32_t experience;
};

struct PlayerListEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::PlayerListEvent;

public:
  EventOpcode getCode() const { return opCode; }
  std::vector<PlayerInfoDTO> players;
};

#endif
