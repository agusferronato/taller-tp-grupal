#include "Desert.h"
#include "NPC/Elf.h"
#include "NPC/SpecialElf.h"
#include "NPC/Skeleton.h"
#include "NPC/SpecialSkeleton.h"
#include "NPC/Golem.h"

void Desert::NPCgenerationStrategy(Game& game) {

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

    trySpawn(0.15, [](Position p) { return std::make_unique<Elf>(p); });
    trySpawn(0.15, [](Position p) { return std::make_unique<SpecialElf>(p); });
    trySpawn(0.15, [](Position p) { return std::make_unique<Skeleton>(p); });
    trySpawn(0.15, [](Position p) { return std::make_unique<SpecialSkeleton>(p); });
    trySpawn(0.05, [](Position p) { return std::make_unique<Golem>(p); });
}

void Desert::decreaseCounter() {
    npcCounter--;
}
