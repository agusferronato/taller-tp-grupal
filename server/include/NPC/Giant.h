#ifndef GIANT_H
#define GIANT_H

#include "NPC.h"

class Giant : public NPC {
public:
    Giant(Position position) : NPC(position) { }
    NPCType getType() override { return NPCType::GiantT; }

    int getAncho() const override { return 32; };
    int getAlto() const override { return 128; };
    int getRange() const override { return 128; };

    std::string getName() override { return "Giant"; }
    int getAttackCounterMax() override { return 100; }
    uint32_t getDamage() override { return 25; }
};

#endif
