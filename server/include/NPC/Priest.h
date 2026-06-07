#ifndef PRIEST_H
#define PRIEST_H

#include "CityEntity.h"

class Priest : public CityEntity {
public:
    Priest(Position position) : CityEntity(position) {}

    CityEntityType getCityEntityType() override {
        return CityEntityType::Priest;
    }

    int getAncho() const override { return 32; }
    int getAlto() const override { return 64; }
};

#endif
