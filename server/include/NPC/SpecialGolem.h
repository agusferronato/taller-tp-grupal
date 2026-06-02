#ifndef SPECIALGOLEM_H
#define SPECIALGOLEM_H

#include "NPC.h"

class SpecialGolem : public NPC {
public:
    SpecialGolem(Position position) : NPC(position) { }
    NPCType getType() override { return NPCType::SpecialGolemT; }
};

#endif
