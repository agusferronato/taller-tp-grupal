#ifndef BIOMEDATA_H
#define BIOMEDATA_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

struct BiomeNPCEntry {
  uint8_t npcId;
  double probability;
};

struct BiomeInfo {
  unsigned int maxNPC;
  std::vector<BiomeNPCEntry> npcs;
};

class BiomeData {
private:
  std::unordered_map<std::string, BiomeInfo> biomes;

public:
  explicit BiomeData(const std::string &path);

  const BiomeInfo &getBiomeInfo(const std::string &type) const;
  unsigned int getMaxNPC(const std::string &type) const;
  const std::vector<BiomeNPCEntry> &getNPCsFor(const std::string &type) const;
};

#endif
