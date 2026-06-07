#ifndef SPECIALGOLEM_H
#define SPECIALGOLEM_H

#include "NPC.h"
#include "string"

class SpecialGolem : public NPC {
public:
  explicit SpecialGolem(Position position) : NPC(position) {}
  NPCType getType() override { return NPCType::SpecialGolemT; }

  const std::string &getName() const override {
    static const std::string name = "Special Golem";
    return name;
  }

  int getAncho() const override { return 32; };
  int getAlto() const override { return 96; };
  int getRange() const override { return 128; };
};

#endif
