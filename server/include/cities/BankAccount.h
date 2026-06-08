#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

#include <map>
#include <cstdint>
#include <CityEntityException.h>

struct ItemInBank {
    int amount{0};
};


class BankAccount {

private:
    std::map<uint8_t, ItemInBank> items;
    uint16_t gold;

public:

    void takeItem(uint8_t item) {

        auto it = items.find(item);
        if (it == items.end())
            throw ItemNotFoundInBank();

        it->second.amount--;

        if (!it->second.amount)
            items.erase(item);
    }


    void saveItem(uint8_t item) {

        auto it = items.find(item);
        if (it == items.end()) {
            items.insert({
                item,
                ItemInBank{}
            });
        };

        it->second.amount++;
    }

    
    void takeGold(uint16_t ammount) {

        if (ammount > gold)
            throw NoGoldAvailable();

        gold -= ammount;

    }

    void saveGold(uint16_t ammount) {
        gold += ammount;
    }


    const std::map<uint8_t, ItemInBank>& getItems() const {
        return items;
    }


    uint16_t getGold() {
        return gold;
    }

};



#endif 
