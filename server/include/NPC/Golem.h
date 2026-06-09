#ifndef GOLEM_H
#define GOLEM_H

#include "NPC.h"
#include "string"

class Golem : public NPC {
public:
  explicit Golem(Position position) : NPC(position) {
    level = 8 + std::rand() % 9;
    maxHp = 100 + std::rand() % 101;
    hp = maxHp;
    agility = 4;
  }
  NPCType getType() override { return NPCType::GolemT; }

    int getAncho() const override { return 32; };
    int getAlto() const override { return 96; };
    int getRange() const override { return 128; };

    std::string getName() override { return "Golem"; }
    int getAttackCounterMax() override { return 225; }
    uint32_t getDamage() override { return 2; }
};

#endif
