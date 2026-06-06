#ifndef NPC_H
#define NPC_H

#include "Position.h"
#include "NPCType.h"


class NPC {

protected:
    Position gridPosition;
    int range{128};
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


    void updatePosition(Game& game, Character& character) {

        if (abs(x - character.getX()) <= range && abs(y - character.getY()) <= range) {

            int vx =  character.getX() - x;
            int vy = character.getY() - y;
            int norm = vx*vx + vy*vy;
            if (norm == 0) return;

            float cos_a = (float)vx / norm; 

            if (cos_a > 0.71f) {
                
                // right
                /*
                this->x += 1; 
                game.moveNPCTo(direction = left, *this);
                */

            } else if (cos_a < -0.71f) {
                //left
            } else if (vy > 0) {
                //down
            } else {
                //up
            }

        }

    }


};


#endif


