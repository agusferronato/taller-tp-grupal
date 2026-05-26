#ifndef PLAYER_LIST_DTO_H
#define PLAYER_LIST_DTO_H

#include "Direction.h"
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
  std::string race;
};

struct PlayerListEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::PlayerListEvent;

public:
  EventOpcode getCode() const { return opCode; }
  std::vector<PlayerInfoDTO> players;
};

#endif
