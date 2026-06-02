#ifndef DESERT_H
#define DESERT_H

#include "biomes/Biome.h"
#include "Game.h"

#define GENERATE_NPC_COUNTER 120
#define MAX_NPC 15

class Desert : public Biome {

private:
    unsigned int counter = 0, npcCounter = 0;

public:

    Desert(Delimiter init, Delimiter end) : Biome(init, end) { }

    void NPCgenerationStrategy(Game& game) override;

    void decreaseCounter();

};

#endif
