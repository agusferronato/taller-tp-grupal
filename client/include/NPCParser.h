#ifndef NPC_PARSER_H
#define NPC_PARSER_H

#include "NPCType.h"
#include "TextureLayoutType.h"

struct NPCInfo {
  int textureId;
  TextureLayoutType layoutType;
};

class NPCParser {
public:
  NPCInfo getInfo(NPCType npcType) const;
};

#endif
