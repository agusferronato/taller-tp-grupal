#ifndef GOLEM_H
#define GOLEM_H

#include "NPC.h"

class Golem : public NPC {
public:
    Golem(Position position) : NPC(position) { }
    NPCType getType() override { return NPCType::GolemT; }

    int getAncho() const override { return 32; };
    int getAlto() const override { return 96; };
    int getRange() const override { return 128; };
};

#endif
