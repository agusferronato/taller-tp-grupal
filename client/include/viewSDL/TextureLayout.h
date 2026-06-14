#ifndef TEXTURE_LAYOUT_H
#define TEXTURE_LAYOUT_H

#include <variant>

#include "AttackLayout.h"
#include "BodyLayout.h"
#include "HeadLayout.h"
#include "TextureLayoutType.h"

using TextureLayout = std::variant<BodyLayout, HeadLayout, AttackLayout>;

#endif 
