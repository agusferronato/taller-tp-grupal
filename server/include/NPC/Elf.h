#ifndef ELF_H
#define ELF_H

#include "NPC.h"

class Elf : public NPC {
public:
    Elf(Position position) : NPC(position) { }
    NPCType getType() override { return NPCType::ElfT; }
};

#endif
