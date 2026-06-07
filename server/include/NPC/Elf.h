#ifndef ELF_H
#define ELF_H

#include "NPC.h"
#include "string"

class Elf : public NPC {

public:
  explicit Elf(Position position) : NPC(position) {}
  NPCType getType() override { return NPCType::ElfT; }

  const std::string &getName() const override {
    static const std::string name = "Elf";
    return name;
  }

  int getAncho() const override { return 32; };
  int getAlto() const override { return 32; };
  int getRange() const override { return 128; };
};

#endif
