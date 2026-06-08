#ifndef SPECIALSPIDER_H
#define SPECIALSPIDER_H

#include "NPC.h"
#include "string"

class SpecialSpider : public NPC {
public:
  explicit SpecialSpider(Position position) : NPC(position) {}
  NPCType getType() override { return NPCType::SpecialSpiderT; }

  const std::string &getName() const override {
    static const std::string name = "Special Spider";
    return name;
  }

  int getAncho() const override { return 96; };
  int getAlto() const override { return 64; };
  int getRange() const override { return 128; };
};

#endif
