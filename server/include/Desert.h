#ifndef DESERT_H
#define DESERT_H

#include "Biome.h"
#include "Zombie.h"


#define NPC_APPEARED_PROBABILITY 0.05
#define GENERATE_ZOMBIE_COUNTER 120


class Desert : public Biome {

private:

    unsigned int counter = 0;

public:

    Desert(Delimiter init, Delimiter end) : Biome(init, end) { };

    void NPCgenerationStrategy(Game& game) override {

        counter = (counter + 1) % GENERATE_ZOMBIE_COUNTER;

        if (counter == GENERATE_ZOMBIE_COUNTER && getRandomNumber() <= NPC_APPEARED_PROBABILITY) {
            
            Position position = getRandomPositionBetween(start, end);

            if (!game.thereIsACollidableEntityAt(position)) {

                std::unique_ptr<Zombie> zombie = std::make_unique<Zombie>(position);
                game.appearNPC(std::move(zombie));
            
            }
        }

    };


};



#endif
