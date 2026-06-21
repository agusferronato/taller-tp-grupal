#include "Trader.h"
#include "Game.h"
#include "ItemData.h"

std::map<uint8_t, ItemInStore> Trader::buildStoreItems(const std::map<uint8_t, StoreItemEntry>& entries) {
    std::map<uint8_t, ItemInStore> items;
    for (const auto& [id, entry] : entries) {
        const PriceData& price = ItemData::instance().getPriceData(id);
        items[id] = {price.buyPrice, price.sellPrice, entry.stock};
    }
    return items;
}

CityEntityType Trader::getCityEntityType() { return CityEntityType::Trader; }

int Trader::getAncho() const { return TRADER_WIDTH; }
int Trader::getAlto() const { return TRADER_HEIGHT; }

Trader::Trader(Position position, const EntityStoreData& storeData)
    : CityEntity(position),
      store(buildStoreItems(storeData.items), storeData.itemsNotForTrading) {}

void Trader::buyItem(Game& game, Character& character, uint8_t itemId) {
    try {
        store.buyItemWith(character, itemId);
        game.sendSystemMessage(character.getId(),
            "Has comprado " + ItemData::instance().getItemName(itemId) + " por " +
            std::to_string(getStoreItem(itemId).purchase_price) + " de oro.");
        game.sendInventoryUpdate(character.getId());
    } catch (const ItemNotAvailable& e) {
        game.sendSystemMessage(character.getId(), e.what());
    } catch (const InsufficientGold& e) {
        game.sendSystemMessage(character.getId(), e.what());
    }
}

void Trader::sellItem(Game& game, Character& character, uint8_t itemId) {
    try {
        store.sellItem(character, itemId);
        game.sendSystemMessage(character.getId(),
            "Has vendido " + ItemData::instance().getItemName(itemId) + " por " +
            std::to_string(getStoreItem(itemId).sell_price) + " de oro.");
        game.sendInventoryUpdate(character.getId());
    } catch (const PlayerDoesNotHaveTheItem& e) {
        game.sendSystemMessage(character.getId(), e.what());
    } catch (const ItemNotAvailable& e) {
        game.sendSystemMessage(character.getId(), e.what());
    }
}

void Trader::listItems(Game& game, Character& character) { 
    const auto& items = store.getItems();
    if (items.empty()) {
        game.sendSystemMessage(character.getId(),
            "El comerciante no tiene objetos disponibles.");
        return;
    }
    std::string msg = "Objetos del comerciante: \n";
    for (auto& [id, item] : items) {
        msg += "    (# " + std::to_string(id) + ") " + ItemData::instance().getItemName(id)
            + " (x" + std::to_string(item.stock) + "). Compra: " 
            + std::to_string(item.purchase_price) + " Venta: " 
            + std::to_string(item.sell_price) + " \n";

    }
    msg.erase(msg.size() - 2);
    game.sendSystemMessage(character.getId(), msg);
}

