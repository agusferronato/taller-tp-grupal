#ifndef SPECIALELF_H
#define SPECIALELF_H

#include "NPC.h"

class SpecialElf : public NPC {
public:
    SpecialElf(Position position) : NPC(position) { }
    NPCType getType() override { return NPCType::SpecialElfT; }

    int getAncho() const override { return 32; };
    int getAlto() const override { return 32; };
    int getRange() const override { return 128; };
    
};

#endif
