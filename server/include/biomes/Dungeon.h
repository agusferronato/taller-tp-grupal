#ifndef DUNGEON_H
#define DUNGEON_H

#include "Biome.h"

#define GENERATE_NPC_COUNTER 240
#define MAX_NPC 10

class Dungeon : public Biome {

private:
    unsigned int counter = 0;

public:

    Dungeon(Delimiter init, Delimiter end) : Biome(init, end) { }

    void NPCgenerationStrategy(Game& game) override;

};

#endif
