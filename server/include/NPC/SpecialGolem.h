#ifndef SPECIALGOLEM_H
#define SPECIALGOLEM_H

#include "NPC.h"
#include "string"

class SpecialGolem : public NPC {
public:
  explicit SpecialGolem(Position position) : NPC(position) {}
  NPCType getType() override { return NPCType::SpecialGolemT; }

    int getAncho() const override { return 32; };
    int getAlto() const override { return 96; };
    int getRange() const override { return 128; };

    std::string getName() override { return "SpecialGolem"; }
    int getAttackCounterMax() override { return 220; }
    uint32_t getDamage() override { return 2; }
};

#endif
