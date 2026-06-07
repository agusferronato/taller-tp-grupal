#ifndef GREATREAMER_H
#define GREATREAMER_H

#include "NPC.h"
#include "string"

class GreatReamer : public NPC {
public:
  explicit GreatReamer(Position position) : NPC(position) {}
  NPCType getType() override { return NPCType::GreatReamerT; }

    int getAncho() const override { return 64; };
    int getAlto() const override { return 64; };
    int getRange() const override { return 128; };

    std::string getName() override { return "GreatReamer"; }
    int getAttackCounterMax() override { return 250; }
    uint32_t getDamage() override { return 1; }
};

#endif
