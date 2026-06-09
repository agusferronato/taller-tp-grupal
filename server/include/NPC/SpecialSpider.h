#ifndef SPECIALSPIDER_H
#define SPECIALSPIDER_H

#include "NPC.h"
#include "string"

class SpecialSpider : public NPC {
public:
  explicit SpecialSpider(Position position) : NPC(position) {
    level = 6 + std::rand() % 7;
    maxHp = 60 + std::rand() % 61;
    hp = maxHp;
    agility = 15;
  }
  NPCType getType() override { return NPCType::SpecialSpiderT; }

    int getAncho() const override { return 96; };
    int getAlto() const override { return 64; };
    int getRange() const override { return 128; };

    std::string getName() override { return "SpecialSpider"; }
    int getAttackCounterMax() override { return 150; }
    uint32_t getDamage() override { return 2; }
};

#endif
