#ifndef CITY_ENTITY_H
#define CITY_ENTITY_H

#include "CityEntityType.h"
#include "Colisionable.h"
#include "Direction.h"
#include "Position.h"

class Character;

class CityEntity : public Colisionable {

protected:
    uint32_t id;
    Position gridPosition;
    int range{80};
    int x, y;
    Direction direction{Direction::Down};
    bool isMoving{false};

public:
    CityEntity(Position pos) : gridPosition(pos), x(0), y(0) {}

    virtual ~CityEntity() = default;

    virtual CityEntityType getCityEntityType() = 0;

    void setId(uint32_t newId) { id = newId; }
    uint32_t getId() const { return id; }
    const Position& getPosition() const { return gridPosition; }
    int getX() const override { return x; }
    int getY() const override { return y; }

    virtual int getAncho() const = 0;
    virtual int getAlto() const = 0;
    virtual int getRange() const = 0;

    Direction getDirection() const { return direction; }
    bool getIsMoving() const { return isMoving; }
    void setPixelPosition(int px, int py) { x = px; y = py; }
    void stop() { isMoving = false; }

    bool colisionaCon(int targetX, int targetY, int targetAncho,
                      int targetAlto) const override;

    bool updatePosition(const Character& character);
};

#endif
