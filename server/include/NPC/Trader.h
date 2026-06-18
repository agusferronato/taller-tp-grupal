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

    CityEntityType getCityEntityType() override;

    int getAncho() const override;
    int getAlto() const override;

    void buyItem(Game& game, Character& character, uint8_t itemId);
    void sellItem(Game& game, Character& character, uint8_t itemId);
    void listItems(Game& game, Character& character);
};

#endif
