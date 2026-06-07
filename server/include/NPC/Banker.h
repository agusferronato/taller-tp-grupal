#ifndef BANKER_H
#define BANKER_H

#include "CityEntity.h"

class Banker : public CityEntity {
public:
    Banker(Position position) : CityEntity(position) {}

    CityEntityType getCityEntityType() override {
        return CityEntityType::Banker;
    }

    int getAncho() const override { return 32; }
    int getAlto() const override { return 64; }
};

#endif
