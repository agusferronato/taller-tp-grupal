#ifndef BANKER_H
#define BANKER_H

#include "CityEntity.h"
#include <Bank.h>

class Game;
class Character;

class Banker : public CityEntity {

private:
    Bank bank;

public:

    Banker(Position position) : CityEntity(position) {}

    CityEntityType getCityEntityType() override {
        return CityEntityType::Banker;
    }

    void saveItem(Game& game, Character& character, uint8_t id);
    void takeItem(Game& game, Character& character, uint8_t id);
    void saveGold(Game& game, Character& character, uint16_t ammount);
    void takeGold(Game& game, Character& character, uint16_t ammount);
    void showGoldAvailable(Game& game, Character& character);
    void listItemsAvailables(Game& game, Character& character);

    int getAncho() const override { return 32; }
    int getAlto() const override { return 64; }
};

#endif
