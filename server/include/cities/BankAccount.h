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

    void takeItem(uint8_t item);

    void saveItem(uint8_t item);
    
    void takeGold(uint16_t ammount);

    void saveGold(uint16_t ammount);

    const std::map<uint8_t, ItemInBank>& getItems() const;

    uint16_t getGold();

};



#endif 
