#include "Bank.h"
#include "Character.h"

void Bank::takeItem(Character& character, uint8_t item) {
    createAccountIfDoesNotExists(character);
    auto account = accounts.find(character.getId());

    if (character.addItem(item))
        account->second.takeItem(item);
}

void Bank::takeGold(Character& character, uint16_t ammount) {
    createAccountIfDoesNotExists(character);
    auto account = accounts.find(character.getId());

    account->second.takeGold(ammount);
    character.addGold(ammount);
}

void Bank::saveGold(Character& character, uint16_t ammount) {
    if (!character.hasMoney(ammount))
        throw InsufficientGold();

    createAccountIfDoesNotExists(character);
    auto account = accounts.find(character.getId());
    account->second.saveGold(ammount);
    character.decreaseGold(ammount);
}

void Bank::saveItem(Character& character, uint8_t item) {
    if (!character.hasItem(item))
        throw PlayerDoesNotHaveTheItem();

    createAccountIfDoesNotExists(character);
    auto account = accounts.find(character.getId());
    account->second.saveItem(item);
    character.removeItemById(item);
}

uint16_t Bank::getGold(Character& character) {
    createAccountIfDoesNotExists(character);
    auto account = accounts.find(character.getId());
    return account->second.getGold();
}

const std::map<uint8_t, ItemInBank>& Bank::getItems(Character& character) {
    createAccountIfDoesNotExists(character);
    auto account = accounts.find(character.getId());
    return account->second.getItems();
}

void Bank::createAccountIfDoesNotExists(Character& character) {
    uint32_t id = character.getId();
    if (!haveAnAccount(id)) {
        accounts.insert({id, BankAccount()});
    }
}
