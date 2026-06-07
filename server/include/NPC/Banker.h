#ifndef BANKER_H
#define BANKER_H

#include "CityEntity.h"
#include <Bank.h>

class Banker : public CityEntity {

private:
    Bank bank;


public:

    Banker(Position position) : CityEntity(position) {}

    CityEntityType getCityEntityType() override {
        return CityEntityType::Banker;
    }


    void saveItem(Game& game, Character& character, uint8_t id) {

        try {
            bank.saveItem(character, id);

        } catch (const PlayerDoesNotHaveTheItem& e) {
            // send msg
        }

    }


    void takeItem(Game& game, Character& character, uint8_t id) {

        try {
            
            bank.takeItem(character, id);

        } catch (const ItemNotFoundInBank& e) {
            // send msg
        }
    }


    void saveGold (Game& game, Character& character, uint16_t ammount) {

        try {
            bank.saveGold(character, ammount);

        } catch (const InsufficientGold& e) {
            // send msg
        }
    }

    void takeGold (Game& game, Character& character, uint16_t ammount) {

        try {
            bank.takeGold(character, ammount);
        
        } catch (const NoGoldAvailable& e) {

            //msg...
        }
    }


    void showGoldAvailable(Game& game, Character& character) {

        uint16_t ammount = bank.getGold(character);
        // send msg

    }

    void listItemsAvailables(Game& game, Character& character) {

        const std::map<uint8_t, ItemInBank>& items = bank.getItems();
        
        for (auto& [id, itemInBank] : items) {
            //show...
        }

    }




    int getAncho() const override { return 32; }
    int getAlto() const override { return 64; }
};

#endif
