#ifndef CLIENT_CITY_ENTITY_MODEL_H
#define CLIENT_CITY_ENTITY_MODEL_H

#include "Direction.h"

class CityEntityModel {
private:
    int x, y;
    Direction direction;
    bool isMoving;

public:
    CityEntityModel(int x, int y, Direction dir = Direction::Down);

    int get_x() const;
    int get_y() const;
    Direction getDirection() const;
    bool getIsMoving() const;

    void setCoordinates(int x, int y);
    void updateCoordinates(int x, int y, Direction direction);
    void stopMoving();
};

#endif
