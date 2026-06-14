#ifndef SPIDER_H
#define SPIDER_H

#include "NPC.h"
#include "string"

class Spider : public NPC {
public:
  explicit Spider(Position position) : NPC(position) {
    level = 2 + std::rand() % 5;
    maxHp = 20 + std::rand() % 41;
    hp = maxHp;
    agility = 12;
  }
  NPCType getType() override { return NPCType::SpiderT; }

    int getAncho() const override { return 48; };
    int getAlto() const override { return 48; };
    int getRange() const override { return 128; };

    std::string getName() override { return "Spider"; }
    int getAttackCounterMax() override { return 100; }
    uint32_t getDamage() override { return 1; }
};

#endif
