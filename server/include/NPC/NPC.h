#ifndef NPC_H
#define NPC_H

#include "Direction.h"
#include "NPCType.h"
#include "Position.h"

class Character;

class NPC {

protected:
    uint16_t id;
    Position gridPosition;
    int range{128};
    int x, y;
    Direction direction{Direction::Down};
    bool isMoving{false};
    static constexpr int ANCHO = 32;
    static constexpr int ALTO = 32;

public:

    NPC(Position pos) : gridPosition(pos), x(0), y(0) { }

    virtual ~NPC() = default;

    virtual NPCType getType() = 0;

    void setId(uint16_t newId) { id = newId; }
    uint16_t getId() const { return id; }
    const Position& getPosition() const { return gridPosition; }
    int getX() const { return x; }
    int getY() const { return y; }
    int getAncho() const { return ANCHO; }
    int getAlto() const { return ALTO; }
    int getRange() const { return range; }
    Direction getDirection() const { return direction; }
    bool getIsMoving() const { return isMoving; }
    void setPixelPosition(int px, int py) { x = px; y = py; }
    void stop() { isMoving = false; }

    bool updatePosition(const Character& character);

};


#endif


