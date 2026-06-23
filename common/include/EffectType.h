#ifndef EFFECT_TYPE_H
#define EFFECT_TYPE_H

#include <cstdint>

enum class EffectType : uint8_t {
  None,
  NormalAttack,
  Bow,
  Explosion,
  Heal,
  Misil
};

#endif
