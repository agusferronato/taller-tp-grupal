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
    CityEntity(Position pos);

    virtual ~CityEntity() = default;

    virtual CityEntityType getCityEntityType() = 0;

    void setId(uint32_t newId);
    uint32_t getId() const;
    const Position& getPosition() const;
    int getX() const override;
    int getY() const override;

    virtual int getAncho() const = 0;
    virtual int getAlto() const = 0;
    int getRange();

    Direction getDirection() const;
    bool getIsMoving() const;
    void setPixelPosition(int px, int py);
    void stop();

    bool colisionaCon(int targetX, int targetY, int targetAncho,
                      int targetAlto) const override;

    bool updatePosition(const Character& character);
};

#endif
