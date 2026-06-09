#include "Forest.h"
#include "Game.h"
#include "NPC/Spider.h"
#include "NPC/SpecialSpider.h"
#include "NPC/SpecialGolem.h"
#include "NPC/Zombie.h"
#include "NPC/Orc.h"
#include "NPC/Skeleton.h"

void Forest::NPCgenerationStrategy(Game& game) {

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

    trySpawn(0.02, [](Position p) { return std::make_unique<Spider>(p); });
    trySpawn(0.02, [](Position p) { return std::make_unique<SpecialSpider>(p); });
    trySpawn(0.02, [](Position p) { return std::make_unique<SpecialGolem>(p); });
    trySpawn(0.02, [](Position p) { return std::make_unique<Zombie>(p); });
    trySpawn(0.02, [](Position p) { return std::make_unique<Orc>(p); });
    trySpawn(0.02, [](Position p) { return std::make_unique<Skeleton>(p); });
}


