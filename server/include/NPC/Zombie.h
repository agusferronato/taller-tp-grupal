#ifndef ZOMBIE_H
#define ZOMBIE_H


#include "NPC.h"

class Zombie : public NPC {

public:

    Zombie(Position position) : NPC(position) { }

    NPCType getType() override {
        return NPCType::ZombieT;
    }

    int getAncho() const override { return 32; };
    int getAlto() const override { return 64; };
    int getRange() const override { return 128; };

};



#endif
