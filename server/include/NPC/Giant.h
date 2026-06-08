#ifndef GIANT_H
#define GIANT_H

#include "NPC.h"
#include "string"

class Giant : public NPC {
public:
  explicit Giant(Position position) : NPC(position) {}
  NPCType getType() override { return NPCType::GiantT; }

  const std::string &getName() const override {
    static const std::string name = "Giant";
    return name;
  }

  int getAncho() const override { return 32; };
  int getAlto() const override { return 128; };
  int getRange() const override { return 128; };
};

#endif
