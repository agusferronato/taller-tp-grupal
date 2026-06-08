#include <algorithm>
#include <cstdlib>

#include "Priest.h"
#include "Game.h"

Priest::Priest(Position position)
    : CityEntity(position),
      store(
          {{{6, {80, 40, 10}},
            {7, {150, 75, 5}},
            {8, {200, 100, 8}},
            {9, {500, 250, 3}},
            {18, {30, 15, 20}},
            {19, {50, 25, 15}}}},
          {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
           19}) {}

void Priest::heal(Game& game, Character& character) {
    character.heal(character.getMaxHp());
    character.addMana(character.getMaxMana());
    game.sendChatToPlayer(character.getId(),
        "El sacerdote te ha curado. Has recuperado toda tu vida y mana.");
    game.sendPlayerInfoUpdate(character.getId());
}

void Priest::resurrect(Game& game, Character& character) {
    
    int distance = sqrt(pow(character.getX() - getX(), 2) + pow(character.getY() - getY(), 2));

    int framesToWait = std::max(distance / 4, 60);

    game.addResurrectingPlayer(character, getX() + 40, getY(), framesToWait);

    game.sendChatToPlayer(character.getId(), "Resucitando...");
}


void Priest::buyItem(Game& game, Character& character, uint8_t itemId) {
    try {
        store.buyItemWith(character, itemId);
        game.sendChatToPlayer(character.getId(),
            "Has comprado " + ITEM_TABLE[itemId].name + " por " +
            std::to_string(getStoreItem(itemId).purchase_price) + " de oro.");
        game.sendInventoryUpdate(character.getId());
    } catch (const ItemNotAvailable& e) {
        game.sendChatToPlayer(character.getId(), e.what());
    } catch (const InsufficientGold& e) {
        game.sendChatToPlayer(character.getId(), e.what());
    }
}

void Priest::listItems(Game& game, Character& character) {
    const auto& items = store.getItems();
    if (items.empty()) {
        game.sendChatToPlayer(character.getId(), 
            "El sacerdote no tiene objetos disponibles.");
        return;
    }
    std::string msg = "Objetos del sacerdote: \n";
    for (auto& [id, item] : items) {
        msg += "    (# " + std::to_string(id) + ") " + ITEM_TABLE[id].name 
            + " (x" + std::to_string(item.stock) + "). Precio de compra: " 
            + std::to_string(item.purchase_price) + " \n";
    }
    msg.erase(msg.size() - 2);
    game.sendChatToPlayer(character.getId(), msg);
}
