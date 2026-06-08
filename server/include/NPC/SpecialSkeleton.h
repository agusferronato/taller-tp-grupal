#ifndef SPECIALSKELETON_H
#define SPECIALSKELETON_H

#include "NPC.h"
#include "string"

class SpecialSkeleton : public NPC {
public:
  explicit SpecialSkeleton(Position position) : NPC(position) {}
  NPCType getType() override { return NPCType::SpecialSkeletonT; }

  const std::string &getName() const override {
    static const std::string name = "Special Skeleton";
    return name;
  }

  int getAncho() const override { return 32; };
  int getAlto() const override { return 64; };
  int getRange() const override { return 128; };
};

#endif
