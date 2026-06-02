#ifndef SPECIALELF_H
#define SPECIALELF_H

#include "NPC.h"

class SpecialElf : public NPC {
public:
    SpecialElf(Position position) : NPC(position) { }
    NPCType getType() override { return NPCType::SpecialElfT; }
};

#endif
