#ifndef MOVE_COMMAND_DTO_H
#define MOVE_COMMAND_DTO_H

#include "Direction.h"
#include <cstdint>

struct MoveCommandDTO {
  uint32_t player_id;
  Direction direction;
};

#endif
