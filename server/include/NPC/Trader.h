#ifndef TRADER_H
#define TRADER_H

#include "CityEntity.h"
#include "Store.h"

class Game;
class Character;

class Trader : public CityEntity {

private:
    Store store;

public:
    Trader(Position position);

    CityEntityType getCityEntityType() override {
        return CityEntityType::Trader;
    }

    int getAncho() const override { return 32; }
    int getAlto() const override { return 64; }

    void buyItem(Game& game, Character& character, uint8_t itemId);
    void sellItem(Game& game, Character& character, uint8_t itemId);
    void listItems(Game& game, Character& character);
};

#endif
