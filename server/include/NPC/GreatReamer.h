#ifndef GREATREAMER_H
#define GREATREAMER_H

#include "NPC.h"

class GreatReamer : public NPC {
public:
    GreatReamer(Position position) : NPC(position) { }
    NPCType getType() override { return NPCType::GreatReamerT; }

    int getAncho() const override { return 64; };
    int getAlto() const override { return 64; };
    int getRange() const override { return 128; };
};

#endif
