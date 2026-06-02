#ifndef SPECIALSPIDER_H
#define SPECIALSPIDER_H

#include "NPC.h"

class SpecialSpider : public NPC {
public:
    SpecialSpider(Position position) : NPC(position) { }
    NPCType getType() override { return NPCType::SpecialSpiderT; }
};

#endif
