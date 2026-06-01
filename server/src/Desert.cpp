#include "Desert.h"
#include "Game.h"

void Desert::NPCgenerationStrategy(Game& game) {

    counter = (counter + 1) % GENERATE_ZOMBIE_COUNTER;

    if (counter == 0 && getRandomNumber() <= NPC_APPEARED_PROBABILITY) {

        Position position = getRandomPositionBetween(start, end);

        if (!game.thereIsACollidableEntityAt(position)) {

            std::unique_ptr<Zombie> zombie = std::make_unique<Zombie>(position);
            game.appearNPC(std::move(zombie));

        }
    }
}
