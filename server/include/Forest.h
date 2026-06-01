#ifndef FOREST_H
#define FOREST_H

#include "Biome.h"

class Forest : public Biome {

public:

    Forest(Delimiter init, Delimiter end) : Biome(init, end) { };

    void NPCgenerationStrategy(Game& game) override;

};

#endif
