#ifndef SPECIALELF_H
#define SPECIALELF_H

#include "NPC.h"
#include "string"

class SpecialElf : public NPC {
public:
  explicit SpecialElf(Position position) : NPC(position) {}
  NPCType getType() override { return NPCType::SpecialElfT; }

  const std::string &getName() const override {
    static const std::string name = "Special Elf";
    return name;
  }

  int getAncho() const override { return 32; };
  int getAlto() const override { return 32; };
  int getRange() const override { return 128; };
};

#endif
