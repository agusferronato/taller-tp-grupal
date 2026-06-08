#ifndef GOLEM_H
#define GOLEM_H

#include "NPC.h"
#include "string"

class Golem : public NPC {
public:
  explicit Golem(Position position) : NPC(position) {}
  NPCType getType() override { return NPCType::GolemT; }

  const std::string &getName() const override {
    static const std::string name = "Golem";
    return name;
  }

  int getAncho() const override { return 32; };
  int getAlto() const override { return 96; };
  int getRange() const override { return 128; };
};

#endif
