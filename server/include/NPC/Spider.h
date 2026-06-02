#ifndef SPIDER_H
#define SPIDER_H

#include "NPC.h"

class Spider : public NPC {
public:
    Spider(Position position) : NPC(position) { }
    NPCType getType() override { return NPCType::SpiderT; }
};

#endif
