#ifndef GIANT_H
#define GIANT_H

#include "NPC.h"
#include "string"

class Giant : public NPC {
public:
  explicit Giant(Position position) : NPC(position) {
    level = 15 + std::rand() % 11;
    maxHp = 200 + std::rand() % 201;
    hp = maxHp;
    agility = 3;
  }
  NPCType getType() override { return NPCType::GiantT; }

    int getAncho() const override { return 32; };
    int getAlto() const override { return 128; };
    int getRange() const override { return 128; };
    std::string getName() override { return "Giant"; }
    int getAttackCounterMax() override { return 100; }
    uint32_t getDamage() override { return 25; }
};

#endif
