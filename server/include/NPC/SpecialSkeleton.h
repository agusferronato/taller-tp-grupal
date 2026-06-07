#ifndef SPECIALSKELETON_H
#define SPECIALSKELETON_H

#include "NPC.h"

class SpecialSkeleton : public NPC {
public:
    SpecialSkeleton(Position position) : NPC(position) { }
    NPCType getType() override { return NPCType::SpecialSkeletonT; }

    int getAncho() const override { return 32; };
    int getAlto() const override { return 64; };
    int getRange() const override { return 128; };

    std::string getName() override { return "SpecialSkeleton"; }
    int getAttackCounterMax() override { return 200; }
    uint32_t getDamage() override { return 3; }
};

#endif
