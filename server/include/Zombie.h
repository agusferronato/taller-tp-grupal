#ifndef ZOMBIE_H
#define ZOMBIE_H


#include "NPC.h"

class Zombie : public NPC {

public:

    Zombie(Position position) : NPC(position) { }

    NPCType getType() override {
        return NPCType::Zombie;
    }

};





#endif
