#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "NPC.h"
#include "string"

class Zombie : public NPC {

public:
  explicit Zombie(Position position) : NPC(position) {
    level = 1 + std::rand() % 5;
    maxHp = 20 + std::rand() % 31;
    hp = maxHp;
    agility = 4;
  }

  NPCType getType() override { return NPCType::ZombieT; }


    int getAncho() const override { return 32; };
    int getAlto() const override { return 64; };
    int getRange() const override { return 128; };
    std::string getName() override { return "Zombie"; }
    int getAttackCounterMax() override { return 130; }
    uint32_t getDamage() override { return 1; }
};

#endif
