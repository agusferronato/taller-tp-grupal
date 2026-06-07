#ifndef SPIDER_H
#define SPIDER_H

#include "NPC.h"
#include "string"

class Spider : public NPC {
public:
  explicit Spider(Position position) : NPC(position) {}
  NPCType getType() override { return NPCType::SpiderT; }

    int getAncho() const override { return 48; };
    int getAlto() const override { return 48; };
    int getRange() const override { return 128; };

    std::string getName() override { return "Spider"; }
    int getAttackCounterMax() override { return 100; }
    uint32_t getDamage() override { return 1; }
};

#endif
