#ifndef SPIDER_H
#define SPIDER_H

#include "NPC.h"

class Spider : public NPC {
public:
    Spider(Position position) : NPC(position) { }
    NPCType getType() override { return NPCType::SpiderT; }

    int getAncho() const override { return 32; };
    int getAlto() const override { return 32; };
    int getRange() const override { return 128; };
};

#endif
