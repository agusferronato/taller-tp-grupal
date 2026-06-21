#ifndef EFFECT_PARSER_H
#define EFFECT_PARSER_H

#include "EffectType.h"
#include "TextureLayoutType.h"

class EffectParser {
public:
  TextureLayoutType getEffectLayout(EffectType type) const;
  int getEffectID(EffectType type) const;
};

#endif
