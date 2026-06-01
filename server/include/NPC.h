#ifndef NPC_H
#define NPC_H

#include "Position.h"

enum NPCType {
    ZombieT,
    SpiderT
};


class NPC {

protected:
    Position gridPosition;
    int x, y;
    static constexpr int ANCHO = 32;
    static constexpr int ALTO = 32;

public:

    NPC(Position pos) : gridPosition(pos), x(0), y(0) { }

    virtual ~NPC() = default;

    virtual NPCType getType() = 0;

    const Position& getPosition() const { return gridPosition; }
    int getX() const { return x; }
    int getY() const { return y; }
    int getAncho() const { return ANCHO; }
    int getAlto() const { return ALTO; }
    void setPixelPosition(int px, int py) { x = px; y = py; }

};


#endif


