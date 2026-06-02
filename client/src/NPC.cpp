#include "NPC.h"

NPC::NPC(int x, int y, Direction dir)
    : x(x), y(y), direction(dir), isMoving(false) {}

int NPC::get_x() const { return x; }
int NPC::get_y() const { return y; }

void NPC::setCoordinates(int x, int y) {
    this->x = x;
    this->y = y;
}

void NPC::updateCoordinates(int x, int y, Direction direction) {
    setCoordinates(x, y);
    isMoving = true;
    if (direction != this->direction)
        this->direction = direction;
}

void NPC::stopMoving() { isMoving = false; }
