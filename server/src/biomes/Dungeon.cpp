#include "Dungeon.h"
#include "Game.h"
#include "NPC/GreatReamer.h"
#include "NPC/Giant.h"

void Dungeon::NPCgenerationStrategy(Game& game) {

    if (npcCounter > MAX_NPC)
        return;

    counter = (counter + 1) % GENERATE_NPC_COUNTER;

    if (counter != 0)
        return;

    auto trySpawn = [&](double probability, auto&& createNPC) {
        if (getRandomNumber() <= probability) {
            Position pos = getRandomPositionBetween(start, end);
            if (!game.thereIsACollidableEntityAt(pos)) {
                game.appearNPC(std::move(createNPC(pos)));
            }
        }
    };

    trySpawn(0.05, [](Position p) { return std::make_unique<GreatReamer>(p); });
    trySpawn(0.05, [](Position p) { return std::make_unique<Giant>(p); });
}

void Dungeon::decreaseCounter() {
    npcCounter--;
}
