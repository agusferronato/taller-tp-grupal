#ifndef SKELETON_H
#define SKELETON_H

#include "NPC.h"
#include "string"

class Skeleton : public NPC {
public:
  explicit Skeleton(Position position) : NPC(position) {
    level = 3 + std::rand() % 6;
    maxHp = 30 + std::rand() % 51;
    hp = maxHp;
    agility = 5;
  }
  NPCType getType() override { return NPCType::SkeletonT; }

    int getAncho() const override { return 32; };
    int getAlto() const override { return 64; };
    int getRange() const override { return 128; };

    std::string getName() override { return "Skeleton"; }
    int getAttackCounterMax() override { return 260; }
    uint32_t getDamage() override { return 2; }
};

#endif
