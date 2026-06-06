#ifndef GIANT_H
#define GIANT_H

#include "NPC.h"

class Giant : public NPC {
public:
    Giant(Position position) : NPC(position) { }
    NPCType getType() override { return NPCType::GiantT; }

    int getAncho() const override { return 32; };
    int getAlto() const override { return 64; };
    int getRange() const override { return 128; };

};

#endif
