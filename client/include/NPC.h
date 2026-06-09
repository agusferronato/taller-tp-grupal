#ifndef CLIENT_NPC_H
#define CLIENT_NPC_H

#include "Direction.h"

class NPC {

private:
    int x, y;
    Direction direction;
    bool isMoving;
    mutable bool beingAttacked{false};

public:

    NPC(int x, int y, Direction dir = Direction::Down);

    int get_x() const;
    int get_y() const;
    Direction getDirection() const { return direction; }
    bool getIsMoving() const { return isMoving; }

    void setCoordinates(int x, int y);
    void updateCoordinates(int x, int y, Direction direction);
    void stopMoving();

    bool isBeingAttacked() const { return beingAttacked; }
    void setBeingAttacked(bool v) { beingAttacked = v; }
    void stopAttackEffect() const { beingAttacked = false; }
};

#endif
