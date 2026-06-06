#ifndef SPECIALSKELETON_H
#define SPECIALSKELETON_H

#include "NPC.h"

class SpecialSkeleton : public NPC {
public:
    SpecialSkeleton(Position position) : NPC(position) { }
    NPCType getType() override { return NPCType::SpecialSkeletonT; }

    int getAncho() const override { return 32; };
    int getAlto() const override { return 32; };
    int getRange() const override { return 128; };
};

#endif
