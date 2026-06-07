#ifndef ORC_H
#define ORC_H

#include "NPC.h"
#include "string"

class Orc : public NPC {
public:
  explicit Orc(Position position) : NPC(position) {}
  NPCType getType() override { return NPCType::OrcT; }

  const std::string &getName() const override {
    static const std::string name = "Orc";
    return name;
  }

  int getAncho() const override { return 48; };
  int getAlto() const override { return 96; };
  int getRange() const override { return 128; };
};

#endif
