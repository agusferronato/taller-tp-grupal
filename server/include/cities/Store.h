#ifndef STORE_H
#define STORE_H

#include <map>
#include <set>
#include <cstdint>
#include <ItemDef.h>
#include <Character.h>


class Store {

private:
    std::map<uint8_t, ItemInStore> items;
    std::set<uint8_t> itemsNotAvailablesForTrading;

public:

    Store(
        std::map<uint8_t, ItemInStore> items,
        std::set<uint8_t> itemsNotAvailablesForTrading
    ) : items(std::move(items)), 
        itemsNotAvailablesForTrading(std::move(itemsNotAvailablesForTrading)) { }


    void buyItemWith(Character& character, uint8_t item) {

        auto it = items.find(item);
        if (it == items.end()) 
            throw ItemNotAvailable();

        uint16_t purchase_price = it->second.purchase_price;

        if (purchase_price > character.getGold()) 
            throw InsufficientGold();

        it->second.stock--;

        if (!it->second.stock)
            items.erase(item);
        
        character.decreaseGold(it->second.purchase_price);
        character.addItem(item);
    }


    void sellItem(Character& character, uint8_t item) {

        if (!character.hasItem(item))
            throw PlayerDoesNotHaveTheItem();

        if (!isAvailableForTrading(item))
            throw ItemNotAvailable();

        auto it = items.find(item);
        if (it == items.end()) {
            items.insert({
                item,
                getStoreItem(item) /* 0 stock initially */
            });
        }

        it->second.stock++;

        character.removeItem(item);
        character.increaseGold(it->second.sell_price);
    }


private:

    bool isAvailableForTrading(uint8_t item) {
        auto it = itemsNotAvailablesForTrading.find(item);
        return it == itemsNotAvailablesForTrading.end();
    }

};





#endif
