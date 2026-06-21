#ifndef NPCDATA_H
#define NPCDATA_H

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

#include "NPC.h"

struct NPCInfo {
  std::string name;
  uint32_t agility;
  int attackCounter;
  uint32_t damage;
  int width;
  int height;
  int range;
  uint32_t hp;
  uint32_t level;
};

class NPCData {
private:
  std::unordered_map<uint8_t, NPCInfo> npcs;

public:
  explicit NPCData(const std::string &path);

  std::unique_ptr<NPC> createNPC(uint8_t npcId, Position pos) const;
};

#endif
