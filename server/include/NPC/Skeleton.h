#ifndef SKELETON_H
#define SKELETON_H

#include "NPC.h"

class Skeleton : public NPC {
public:
    Skeleton(Position position) : NPC(position) { }
    NPCType getType() override { return NPCType::SkeletonT; }

    int getAncho() const override { return 128; };
    int getAlto() const override { return 256; };
    int getRange() const override { return 128; };
};

#endif
