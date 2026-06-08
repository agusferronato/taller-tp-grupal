#include "Banker.h"
#include "Game.h"

void Banker::saveItem(Game& game, Character& character, uint8_t id) {
    try {
        bank.saveItem(character, id);
        game.sendInventoryUpdate(character.getId());
    } catch (const PlayerDoesNotHaveTheItem& e) {
        game.sendChatToPlayer(character.getId(), e.what());
    }
}

void Banker::takeItem(Game& game, Character& character, uint8_t id) {
    try {
        bank.takeItem(character, id);
        game.sendInventoryUpdate(character.getId());
    } catch (const ItemNotFoundInBank& e) {
        game.sendChatToPlayer(character.getId(), e.what());
    }
}

void Banker::saveGold(Game& game, Character& character, uint16_t ammount) {
    try {
        bank.saveGold(character, ammount);
        game.sendChatToPlayer(character.getId(),
            "Has depositado " + std::to_string(ammount) +
            " de oro en el banco.");
    } catch (const InsufficientGold& e) {
        game.sendChatToPlayer(character.getId(), e.what());
    }
}

void Banker::takeGold(Game& game, Character& character, uint16_t ammount) {
    try {
        bank.takeGold(character, ammount);
        game.sendChatToPlayer(character.getId(),
            "Has retirado " + std::to_string(ammount) +
            " de oro del banco.");
    } catch (const NoGoldAvailable& e) {
        game.sendChatToPlayer(character.getId(), e.what());
    }
}

void Banker::showGoldAvailable(Game& game, Character& character) {
    uint16_t ammount = bank.getGold(character);
    game.sendChatToPlayer(character.getId(),
        "Tienes " + std::to_string(ammount) +
        " de oro en tu cuenta bancaria.");
}

void Banker::listItemsAvailables(Game& game, Character& character) {
    const std::map<uint8_t, ItemInBank>& items = bank.getItems(character);
    if (items.empty()) {
        game.sendChatToPlayer(character.getId(),
            "No tienes ningun objeto en el banco.");
        return;
    }

    std::string msg = "Tus objetos en el banco: ";
    for (auto& [id, item] : items) {
        msg += ITEM_TABLE[id].name + " x" + std::to_string(item.amount) + ", ";
    }
    msg.erase(msg.size() - 2);
    game.sendChatToPlayer(character.getId(), msg);
}
