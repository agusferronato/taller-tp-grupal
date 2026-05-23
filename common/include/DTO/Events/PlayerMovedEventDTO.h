#ifndef PLAYER_MOVED_EVENT_DTO_H
#define PLAYER_MOVED_EVENT_DTO_H

#include <cstdint>

#include "Direction.h"

struct PlayerMovedEventDTO {
  uint32_t playerId;
  int16_t x;
  int16_t y;
  Direction direction;
};

#endif
