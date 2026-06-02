#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Direction.h"

class Zombie {

public:
    static constexpr int Width = 45;
    static constexpr int Height = 45;

private:
    int x, y;
    Direction direction;
    bool isMoving;

public:
    Zombie(int x, int y, Direction dir = Direction::Down);

    int get_x() const;
    int get_y() const;
    int get_h() const { return Height; }
    Direction getDirection() const { return direction; }
    bool getIsMoving() const { return isMoving; }

    void setCoordinates(int x, int y);
    void updateCoordinates(int x, int y, Direction direction);
    void stopMoving();
};

#endif
