#include "Trader.h"
#include "Game.h"

Trader::Trader(Position position)
    : CityEntity(position),
      store(
          {{{1, {50, 25, 10}},   {2, {80, 40, 8}},
            {3, {60, 30, 10}},   {4, {100, 50, 6}},
            {5, {200, 100, 4}},  {10, {100, 50, 8}},
            {11, {400, 200, 3}}, {12, {200, 100, 5}},
            {13, {50, 25, 10}},  {14, {100, 50, 8}},
            {15, {200, 100, 4}}, {16, {50, 25, 10}},
            {17, {100, 50, 8}},  {18, {30, 15, 20}},
            {19, {50, 25, 15}}}},
          {6, 7, 8, 9}) {}

void Trader::buyItem(Game& game, Character& character, uint8_t itemId) {
    try {
        store.buyItemWith(character, itemId);
        game.sendSystemMessage(character.getId(),
            "Has comprado " + ITEM_TABLE[itemId].name + " por " +
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
            "Has vendido " + ITEM_TABLE[itemId].name + " por " +
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
        msg += "    (# " + std::to_string(id) + ") " + ITEM_TABLE[id].name 
            + " (x" + std::to_string(item.stock) + "). Compra: " 
            + std::to_string(item.purchase_price) + " Venta: " 
            + std::to_string(item.sell_price) + " \n";

    }
    msg.erase(msg.size() - 2);
    game.sendSystemMessage(character.getId(), msg);
}

