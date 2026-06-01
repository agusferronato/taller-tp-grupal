#ifndef SPECIALSKELETON_H
#define SPECIALSKELETON_H

#include "NPC.h"

class SpecialSkeleton : public NPC {
public:
    SpecialSkeleton(Position position) : NPC(position) { }
    NPCType getType() override { return NPCType::SpecialSkeletonT; }
};

#endif
