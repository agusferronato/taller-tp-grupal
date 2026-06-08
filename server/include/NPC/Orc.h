#ifndef ORC_H
#define ORC_H

#include "NPC.h"
#include "string"

class Orc : public NPC {
public:
  explicit Orc(Position position) : NPC(position) {
    level = 5 + std::rand() % 8;
    maxHp = 50 + std::rand() % 71;
    hp = maxHp;
    agility = 6;
  }
  NPCType getType() override { return NPCType::OrcT; }

    int getAncho() const override { return 48; };
    int getAlto() const override { return 96; };
    int getRange() const override { return 128; };

    std::string getName() override { return "Orc"; }
    int getAttackCounterMax() override { return 200; }
    uint32_t getDamage() override { return 2; }
};

#endif
