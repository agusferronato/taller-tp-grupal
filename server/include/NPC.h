#ifndef NPC_H
#define NPC_H

#include "Position.h"

enum NPCType {
    ZombieT,
    SpiderT
};


class NPC {

protected:
    Position position;


public:

    NPC (Position Position) : position(position) { }


    virtual NPCType getType() = 0;


};




#endif

