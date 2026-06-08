#ifndef SPIDER_H
#define SPIDER_H

#include "NPC.h"
#include "string"

class Spider : public NPC {
public:
  explicit Spider(Position position) : NPC(position) {}
  NPCType getType() override { return NPCType::SpiderT; }

  const std::string &getName() const override {
    static const std::string name = "Spider";
    return name;
  }

  int getAncho() const override { return 48; };
  int getAlto() const override { return 48; };
  int getRange() const override { return 128; };
};

#endif
