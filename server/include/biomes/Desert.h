#ifndef DESERT_H
#define DESERT_H

#include "biomes/Biome.h"
#include "Game.h"

#define GENERATE_NPC_COUNTER 240
#define MAX_NPC 10

class Desert : public Biome {

private:
    unsigned int counter = 0;

public:

    Desert(Delimiter init, Delimiter end) : Biome(init, end) { }

    void NPCgenerationStrategy(Game& game) override;

};

#endif
