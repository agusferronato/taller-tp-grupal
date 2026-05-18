#ifndef DIRECTION_H
#define DIRECTION_H

#include <cstdint>

/*
  Direction
  Representa las direcciones en las que un jugador puede moverse.
*/

enum class Direction : uint8_t { Up, Down, Left, Right };

#endif // DIRECTION_H
