#ifndef PRIEST_H
#define PRIEST_H

#include "CityEntity.h"
#include "Store.h"

class Game;
class Character;

class Priest : public CityEntity {

private:
    Store store;

public:
    Priest(Position position);

    CityEntityType getCityEntityType() override {
        return CityEntityType::Priest;
    }

    int getAncho() const override { return 32; }
    int getAlto() const override { return 64; }

    void heal(Game& game, Character& character);
    void resurrect(Game& game, Character& character);
    void buyItem(Game& game, Character& character, uint8_t itemId);
    void listItems(Game& game, Character& character);
};

#endif
