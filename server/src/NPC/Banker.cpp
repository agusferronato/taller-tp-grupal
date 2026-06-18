#include "Banker.h"
#include "Game.h"
#include "ItemData.h"

Banker::Banker(Position position) : CityEntity(position) {}

CityEntityType Banker::getCityEntityType() { return CityEntityType::Banker; }

int Banker::getAncho() const { return BANKER_WIDTH; }
int Banker::getAlto() const { return BANKER_HEIGHT; }

void Banker::saveItem(Game& game, Character& character, uint8_t id) {
    try {
        bank.saveItem(character, id);
        game.sendInventoryUpdate(character.getId());
    } catch (const PlayerDoesNotHaveTheItem& e) {
        game.sendSystemMessage(character.getId(), e.what());
    }
}

void Banker::takeItem(Game& game, Character& character, uint8_t id) {
    try {
        bank.takeItem(character, id);
        game.sendInventoryUpdate(character.getId());
    } catch (const ItemNotFoundInBank& e) {
        game.sendSystemMessage(character.getId(), e.what());
    }
}

void Banker::saveGold(Game& game, Character& character, uint16_t ammount) {
    try {
        bank.saveGold(character, ammount);
        game.sendSystemMessage(character.getId(),
            "Has depositado " + std::to_string(ammount) +
            " de oro en el banco.");
    } catch (const InsufficientGold& e) {
        game.sendSystemMessage(character.getId(), e.what());
    }
}

void Banker::takeGold(Game& game, Character& character, uint16_t ammount) {
    try {
        bank.takeGold(character, ammount);
        game.sendSystemMessage(character.getId(),
            "Has retirado " + std::to_string(ammount) +
            " de oro del banco.");
    } catch (const NoGoldAvailable& e) {
        game.sendSystemMessage(character.getId(), e.what());
    }
}

void Banker::showGoldAvailable(Game& game, Character& character) {
    uint16_t ammount = bank.getGold(character);
    game.sendSystemMessage(character.getId(),
        "Tienes " + std::to_string(ammount) +
        " de oro en tu cuenta bancaria.");
}

void Banker::listItemsAvailables(Game& game, Character& character) { 
    const std::map<uint8_t, ItemInBank>& items = bank.getItems(character);

    if (items.empty()) {
        game.sendSystemMessage(character.getId(),
            "No tienes ningun objeto en el banco.");
        return;
    }

    std::string msg = "Tus objetos en el banco: \n";
    for (auto& [id, item] : items) {
        msg += "    (# " + std::to_string(id) + ") " + ItemData::instance().getItemName(id)
            + " (x" + std::to_string(item.amount) + ") \n";
    }
    msg.erase(msg.size() - 2);
    game.sendSystemMessage(character.getId(), msg);
}
