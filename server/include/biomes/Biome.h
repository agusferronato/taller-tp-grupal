#ifndef BIOME_H
#define BIOME_H

#include <cstdint>
#include <random>
#include <string>
#include <unordered_set>

#include "Position.h"

class BiomeData;
class Game;
class NPCData;

class Biome {

protected:
  std::string type;
  Delimiter start, end;
  std::unordered_set<uint32_t> npcIds;
  unsigned int counter = 0;
  unsigned int maxNPC;

  const BiomeData *biomeData = nullptr;
  const NPCData *npcData = nullptr;

  static constexpr unsigned int GENERATE_NPC_COUNTER = 240;

public:
  Biome(std::string type, Delimiter start, Delimiter end);

  virtual ~Biome() = default;

  void setData(const BiomeData &bd, const NPCData &nd);

  void NPCgenerationStrategy(Game &game);

  void registerNPC(uint32_t id);
  void unregisterNPC(uint32_t id);
  bool hasNPC(uint32_t id) const;
  unsigned int getNPCCount() const;

protected:
  double getRandomNumber();

  Position getRandomPositionBetween(Delimiter start, Delimiter end);
};

#endif
