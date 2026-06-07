#ifndef ELF_H
#define ELF_H

#include "NPC.h"
#include "string"

class Elf : public NPC {

public:
  explicit Elf(Position position) : NPC(position) {}
  NPCType getType() override { return NPCType::ElfT; }

    int getAncho() const override { return 32; };
    int getAlto() const override { return 32; };
    int getRange() const override { return 128; };

    std::string getName() override { return "Elf"; }
    int getAttackCounterMax() override { return 175; }
    uint32_t getDamage() override { return 2; }
};

#endif
