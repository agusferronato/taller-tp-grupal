#include "EffectParser.h"

TextureLayoutType EffectParser::getEffectLayout(EffectType type) const {
  switch (type) {
  case EffectType::Explosion:
    return TextureLayoutType::Explosion;
  case EffectType::Heal:
    return TextureLayoutType::Heal;
  case EffectType::Misil:
    return TextureLayoutType::Misil;
  default:
    return TextureLayoutType::Attack;
  }
}

int EffectParser::getEffectID(EffectType type) const {
  switch (type) {
  case EffectType::Explosion:
    return 351;
  case EffectType::Heal:
    return 352;
  case EffectType::Misil:
    return 353;
  default:
    return 350;
  }
}
