#ifndef GIANT_H
#define GIANT_H

#include "NPC.h"

class Giant : public NPC {
public:
    Giant(Position position) : NPC(position) { }
    NPCType getType() override { return NPCType::GiantT; }
};

#endif
