#ifndef STORE_H
#define STORE_H

#include <map>
#include <set>
#include <cstdint>
#include <ItemDef.h>
#include <CityEntityException.h>

class Character;

struct ItemInStore {
    uint16_t purchase_price;
    uint16_t sell_price;
    int stock{0};
};

inline ItemInStore getStoreItem(uint8_t itemId) {
    static const std::map<uint8_t, std::pair<uint16_t, uint16_t>> prices = {
        {1, {50, 25}}, {2, {80, 40}}, {3, {60, 30}},
        {4, {100, 50}}, {5, {200, 100}}, {6, {80, 40}},
        {7, {150, 75}}, {8, {200, 100}}, {9, {500, 250}},
        {10, {100, 50}}, {11, {400, 200}}, {12, {200, 100}},
        {13, {50, 25}}, {14, {100, 50}}, {15, {200, 100}},
        {16, {50, 25}}, {17, {100, 50}}, {18, {30, 15}},
        {19, {50, 25}}
    };
    auto it = prices.find(itemId);
    if (it != prices.end())
        return {it->second.first, it->second.second, 0};
    return {0, 0, 0};
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
