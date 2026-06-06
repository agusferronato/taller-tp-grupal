#ifndef ORC_H
#define ORC_H

#include "NPC.h"

class Orc : public NPC {
public:
    Orc(Position position) : NPC(position) { }
    NPCType getType() override { return NPCType::OrcT; }

    int getAncho() const override { return 48; };
    int getAlto() const override { return 96; };
    int getRange() const override { return 128; };
};

#endif
