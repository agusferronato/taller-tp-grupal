#ifndef SKELETON_H
#define SKELETON_H

#include "NPC.h"

class Skeleton : public NPC {
public:
    Skeleton(Position position) : NPC(position) { }
    NPCType getType() override { return NPCType::SkeletonT; }
};

#endif
