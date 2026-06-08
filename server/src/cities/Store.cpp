#include "Store.h"
#include "Character.h"

void Store::buyItemWith(Character& character, uint8_t item) {
    auto it = items.find(item);
    if (it == items.end())
        throw ItemNotAvailable();

    uint16_t purchase_price = it->second.purchase_price;

    if (purchase_price > character.getGold())
        throw InsufficientGold();

    if (!character.addItem(item))
        return;

    it->second.stock--;

    if (!it->second.stock)
        items.erase(item);

    character.decreaseGold(purchase_price);
}

void Store::sellItem(Character& character, uint8_t item) {
    if (!character.hasItem(item))
        throw PlayerDoesNotHaveTheItem();

    if (!isAvailableForTrading(item))
        throw ItemNotAvailable();

    auto it = items.find(item);
    if (it == items.end()) {
        auto result = items.insert({
            item,
            getStoreItem(item)
        });
        it = result.first;
    }

    it->second.stock++;

    character.removeItemById(item);
    character.increaseGold(it->second.sell_price);
}
