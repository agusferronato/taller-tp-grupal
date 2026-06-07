#ifndef GREATREAMER_H
#define GREATREAMER_H

#include "NPC.h"
#include "string"

class GreatReamer : public NPC {
public:
  explicit GreatReamer(Position position) : NPC(position) {}
  NPCType getType() override { return NPCType::GreatReamerT; }

  const std::string &getName() const override {
    static const std::string name = "Great Reamer";
    return name;
  }

  int getAncho() const override { return 64; };
  int getAlto() const override { return 64; };
  int getRange() const override { return 128; };
};

#endif
