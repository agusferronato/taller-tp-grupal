#ifndef BANK_H
#define BANK_H


#include <map>
#include <cstdint>
#include <Character.h>
#include "BankAccount.h"


class Bank {


private:

    std::map<uint32_t, BankAccount> accounts;

public:
    
    void takeItem(Character& character, uint8_t item) {

        createAccountIfDoesNotExists(character);

        auto account = accounts.find(character.getId());
        
        account->second.takeItem(item); /* exception handle by top context */
        character.addItem(item);

    }

    void takeGold(Character& character, uint16_t ammount) {

        createAccountIfDoesNotExists(character);

        auto account = accounts.find(character.getId());
        
        account->second.takeGold(ammount); /* exception handle by top context */
        character.addGold(ammount);
    }



    void saveGold(Character& character, uint16_t ammount) {

        if (!character.hasMoney(ammount))
            throw InsufficientGold();

        createAccountIfDoesNotExists(character);

        auto account = accounts.find(character.getId());
        account->second.saveGold(ammount);
        character.decreaseGold(ammount);
    }


    void saveItem(Character& character, uint8_t item) {

        if (!character.hasItem(item))
            throw PlayerDoesNotHaveTheItem();

        createAccountIfDoesNotExists(character);

        auto account = accounts.find(character.getId());
        account->second.saveItem(item); 
    }

    uint16_t getGold(Character& character) {

        createAccountIfDoesNotExists(character);
        auto account = accounts.find(character.getId());

        return account->second.getGold();
    }


    const std::map<uint8_t, ItemInBank>& getItems(Character& character) {

        createAccountIfDoesNotExists(character);
        
        auto account = accounts.find(character.getId());
        return account->second.getItems();
    }




private:

    void createAccountIfDoesNotExists(Character& character) {
        uint32_t id = character.getId();
        if (!haveAnAccount(id)) {
            accounts.insert({
                id, BankAccount()
            });
        }
    }


    bool haveAnAccount(uint32_t player_id) {
        auto it = accounts.find(player_id);
        return !(it == accounts.end());
    }

};




#endif

