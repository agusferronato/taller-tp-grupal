#include "Biome.h"
#include "BiomeData.h"
#include "Game.h"
#include "NPC.h"
#include "NPCData.h"

Biome::Biome(std::string type, Delimiter start, Delimiter end)
    : type(std::move(type)), start(start), end(end), maxNPC(0) {}

void Biome::setData(const BiomeData& bd, const NPCData& nd) {
    biomeData = &bd;
    npcData = &nd;
    maxNPC = biomeData->getMaxNPC(type);
}

void Biome::NPCgenerationStrategy(Game& game) {

    if (getNPCCount() >= maxNPC)
        return;

    counter = (counter + 1) % GENERATE_NPC_COUNTER;

    if (counter != 0)
        return;

    auto trySpawn = [&](double probability, auto&& createNPC) {
        if (getRandomNumber() <= probability) {
            Position pos = getRandomPositionBetween(start, end);
            if (!game.thereIsACollidableEntityAt(pos)) {
                uint32_t npcId = game.appearNPC(std::move(createNPC(pos)));
                registerNPC(npcId);
            }
        }
    };

    for (const auto& entry : biomeData->getNPCsFor(type))
        trySpawn(entry.probability,
            [&](Position p) { return npcData->createNPC(entry.npcId, p); });
}
