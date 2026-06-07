#ifndef TRADER_H
#define TRADER_H

#include "CityEntity.h"

class Trader : public CityEntity {
public:
    Trader(Position position) : CityEntity(position) {}

    CityEntityType getCityEntityType() override {
        return CityEntityType::Trader;
    }

    int getAncho() const override { return 32; }
    int getAlto() const override { return 64; }
    int getRange() const override { return 128; }
};

#endif
