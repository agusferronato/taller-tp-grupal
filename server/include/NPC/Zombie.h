#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "NPC.h"
#include "string"

class Zombie : public NPC {

public:
  explicit Zombie(Position position) : NPC(position) {}

  NPCType getType() override { return NPCType::ZombieT; }

  const std::string &getName() const override {
    static const std::string name = "Zombie";
    return name;
  }

  int getAncho() const override { return 32; };
  int getAlto() const override { return 64; };
  int getRange() const override { return 128; };
};

#endif
