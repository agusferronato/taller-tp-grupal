#ifndef SKELETON_H
#define SKELETON_H

#include "NPC.h"
#include "string"

class Skeleton : public NPC {
public:
  explicit Skeleton(Position position) : NPC(position) {}
  NPCType getType() override { return NPCType::SkeletonT; }

  const std::string &getName() const override {
    static const std::string name = "Skeleton";
    return name;
  }

  int getAncho() const override { return 32; };
  int getAlto() const override { return 64; };
  int getRange() const override { return 128; };
};

#endif
