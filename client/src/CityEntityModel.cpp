#include "CityEntityModel.h"

CityEntityModel::CityEntityModel(int x, int y, Direction dir)
    : x(x), y(y), direction(dir), isMoving(false) {}

int CityEntityModel::get_x() const { return x; }
int CityEntityModel::get_y() const { return y; }

void CityEntityModel::setCoordinates(int x, int y) {
    this->x = x;
    this->y = y;
}

void CityEntityModel::updateCoordinates(int x, int y, Direction direction) {
    setCoordinates(x, y);
    isMoving = true;
    if (direction != this->direction)
        this->direction = direction;
}

void CityEntityModel::stopMoving() { isMoving = false; }
