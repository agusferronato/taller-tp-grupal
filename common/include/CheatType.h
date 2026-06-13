#ifndef CHEAT_TYPE_H
#define CHEAT_TYPE_H

#include <cstdint>

enum class CheatType : uint8_t {
  Die,
  InfiniteHealth,
  NormalHealth,
  InfiniteMana,
  NormalMana,
  SuperSpeed,
  NormalSpeed,
  SetLevel,
  Revive,
};

#endif
