#ifndef ORC_H
#define ORC_H

#include "NPC.h"

class Orc : public NPC {
public:
    Orc(Position position) : NPC(position) { }
    NPCType getType() override { return NPCType::OrcT; }
};

#endif
