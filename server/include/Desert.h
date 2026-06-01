#ifndef DESERT_H
#define DESERT_H

#include "Biome.h"
#include "Zombie.h"


#define NPC_APPEARED_PROBABILITY 0.15
#define GENERATE_ZOMBIE_COUNTER 120


class Desert : public Biome {

private:

    unsigned int counter = 0;

public:

    Desert(Delimiter init, Delimiter end) : Biome(init, end) { };

    void NPCgenerationStrategy(Game& game) override;

};

#endif
