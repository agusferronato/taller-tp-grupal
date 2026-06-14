#ifndef STORE_H
#define STORE_H

#include <map>
#include <set>
#include <cstdint>
#include <CityEntityException.h>
#include <ItemData.h>

class Character;

struct ItemInStore {
    uint16_t purchase_price;
    uint16_t sell_price;
    int stock{0};
};

inline ItemInStore getStoreItem(uint8_t itemId) {
    const PriceData &price = ItemData::instance().getPriceData(itemId);
    return {price.buyPrice, price.sellPrice, 0};
}


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

    const std::map<uint8_t, ItemInStore>& getItems() const { return items; }

    void buyItemWith(Character& character, uint8_t item);
    void sellItem(Character& character, uint8_t item);

private:
    bool isAvailableForTrading(uint8_t item) {
        auto it = itemsNotAvailablesForTrading.find(item);
        return it == itemsNotAvailablesForTrading.end();
    }
};

#endif
