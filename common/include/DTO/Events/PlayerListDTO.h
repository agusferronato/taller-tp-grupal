#ifndef PLAYER_LIST_DTO_H
#define PLAYER_LIST_DTO_H

#include "Direction.h"
#include <cstdint>
#include <vector>

struct PlayerInfoDTO {
  uint32_t player_id;
  int16_t x;
  int16_t y;
  Direction direction;
};

struct PlayerListDTO {
  std::vector<PlayerInfoDTO> players;
};

#endif
