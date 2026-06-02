#ifndef GOLEM_H
#define GOLEM_H

#include "NPC.h"

class Golem : public NPC {
public:
    Golem(Position position) : NPC(position) { }
    NPCType getType() override { return NPCType::GolemT; }
};

#endif
