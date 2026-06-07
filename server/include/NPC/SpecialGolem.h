#ifndef SPECIALGOLEM_H
#define SPECIALGOLEM_H

#include "NPC.h"

class SpecialGolem : public NPC {
public:
    SpecialGolem(Position position) : NPC(position) { }
    NPCType getType() override { return NPCType::SpecialGolemT; }

    int getAncho() const override { return 32; };
    int getAlto() const override { return 96; };
    int getRange() const override { return 128; };
};

#endif
