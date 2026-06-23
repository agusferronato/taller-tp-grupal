#include "Biome.h"
#include "BiomeData.h"
#include "Game.h"
#include "NPC.h"
#include "NPCData.h"

void Biome::registerNPC(uint32_t id) { npcIds.insert(id); }
void Biome::unregisterNPC(uint32_t id) { npcIds.erase(id); }
bool Biome::hasNPC(uint32_t id) const {
  return npcIds.find(id) != npcIds.end();
}
unsigned int Biome::getNPCCount() const { return npcIds.size(); }

double Biome::getRandomNumber() {
  std::mt19937 gen(std::random_device{}());
  std::uniform_real_distribution<double> dist(0.0, 1.0);
  return dist(gen);
}

Position Biome::getRandomPositionBetween(Delimiter start, Delimiter end) {
  std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int> dist1(start.row, end.row);
  std::uniform_int_distribution<int> dist2(start.column, end.column);
  return Position{dist1(gen), dist2(gen)};
}

Biome::Biome(std::string type, Delimiter start, Delimiter end)
    : type(std::move(type)), start(start), end(end), maxNPC(0) {}

void Biome::setData(const BiomeData &bd, const NPCData &nd) {
  biomeData = &bd;
  npcData = &nd;
  maxNPC = biomeData->getMaxNPC(type);
}

void Biome::NPCgenerationStrategy(Game &game) {

  if (getNPCCount() >= maxNPC)
    return;

  counter = (counter + 1) % GENERATE_NPC_COUNTER;

  if (counter != 0)
    return;

  auto trySpawn = [&](double probability, auto &&createNPC, uint8_t npcTypeId) {
    if (getRandomNumber() <= probability) {
      Position pos = getRandomPositionBetween(start, end);
      int w = npcData->getWidth(npcTypeId);
      int h = npcData->getHeight(npcTypeId);
      if (!game.thereIsACollidableEntityAt(pos, w, h)) {
        uint32_t npcId = game.appearNPC(std::move(createNPC(pos)));
        registerNPC(npcId);
      }
    }
  };

  for (const auto &entry : biomeData->getNPCsFor(type))
    trySpawn(
        entry.probability,
        [&](Position p) { return npcData->createNPC(entry.npcId, p); },
        entry.npcId);
}
