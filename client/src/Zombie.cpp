#include "Zombie.h"

Zombie::Zombie(int x, int y, Direction dir)
    : x(x), y(y), direction(dir), isMoving(false) {}

int Zombie::get_x() const { return x; }
int Zombie::get_y() const { return y; }

void Zombie::setCoordinates(int x, int y) {
    this->x = x;
    this->y = y;
}

void Zombie::updateCoordinates(int x, int y, Direction direction) {
    setCoordinates(x, y);
    isMoving = true;
    if (direction != this->direction)
        this->direction = direction;
}

void Zombie::stopMoving() { isMoving = false; }
