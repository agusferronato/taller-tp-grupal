#ifndef BANK_H
#define BANK_H

#include <map>
#include <cstdint>
#include "BankAccount.h"
#include <CityEntityException.h>

class Character;

class Bank {

private:
    std::map<uint32_t, BankAccount> accounts;

public:

    void takeItem(Character& character, uint8_t item);
    void takeGold(Character& character, uint16_t ammount);
    void saveGold(Character& character, uint16_t ammount);
    void saveItem(Character& character, uint8_t item);
    uint16_t getGold(Character& character);
    const std::map<uint8_t, ItemInBank>& getItems(Character& character);

private:
    void createAccountIfDoesNotExists(Character& character);
    bool haveAnAccount(uint32_t player_id) {
        auto it = accounts.find(player_id);
        return !(it == accounts.end());
    }
};

#endif
