#ifndef DUNGEON_H
#define DUNGEON_H

#include "Biome.h"

class Dungeon : public Biome {

public:

    Dungeon(Delimiter init, Delimiter end) : Biome(init, end) { };

    void NPCgenerationStrategy(Game& game) override;

};

#endif
