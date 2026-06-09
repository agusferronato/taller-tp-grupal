#include "Dungeon.h"
#include "Game.h"
#include "NPC/GreatReamer.h"
#include "NPC/Giant.h"

void Dungeon::NPCgenerationStrategy(Game& game) {

    if (getNPCCount() >= MAX_NPC)
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

    trySpawn(0.02, [](Position p) { return std::make_unique<GreatReamer>(p); });
    trySpawn(0.02, [](Position p) { return std::make_unique<Giant>(p); });
}


