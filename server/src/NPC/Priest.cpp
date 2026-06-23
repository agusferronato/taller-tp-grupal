#include <algorithm>
#include <cstdlib>

#include "Game.h"
#include "ItemData.h"
#include "Priest.h"

CityEntityType Priest::getCityEntityType() { return CityEntityType::Priest; }

int Priest::getAncho() const { return PRIEST_WIDTH; }
int Priest::getAlto() const { return PRIEST_HEIGHT; }

Priest::Priest(Position position, const EntityStoreData &storeData)
    : CityEntity(position),
      store(buildStoreItems(storeData.items), storeData.itemsNotForTrading) {}

std::map<uint8_t, ItemInStore>
Priest::buildStoreItems(const std::map<uint8_t, StoreItemEntry> &entries) {
  std::map<uint8_t, ItemInStore> items;
  for (const auto &[id, entry] : entries) {
    const PriceData &price = ItemData::instance().getPriceData(id);
    items[id] = {price.buyPrice, price.sellPrice, entry.stock};
  }
  return items;
}

void Priest::heal(Game &game, Character &character) {
  character.heal(character.getMaxHp());
  character.addMana(character.getMaxMana());
  game.sendSystemMessage(
      character.getId(),
      "El sacerdote te ha curado. Has recuperado toda tu vida y mana.");
  game.sendPlayerInfoUpdate(character.getId());
}

void Priest::resurrect(Game &game, Character &character) {

  int distance = sqrt(pow(character.getX() - getX(), 2) +
                      pow(character.getY() - getY(), 2));

  int framesToWait = std::max(distance / 4, 60);

  game.addResurrectingPlayer(character, getX() + 40, getY(), framesToWait);

  game.sendSystemMessage(character.getId(), "Resucitando...");
}

void Priest::buyItem(Game &game, Character &character, uint8_t itemId) {
  try {
    store.buyItemWith(character, itemId);
    game.sendSystemMessage(
        character.getId(),
        "Has comprado " + ItemData::instance().getItemName(itemId) + " por " +
            std::to_string(getStoreItem(itemId).purchase_price) + " de oro.");
    game.sendInventoryUpdate(character.getId());
  } catch (const ItemNotAvailable &e) {
    game.sendSystemMessage(character.getId(), e.what());
  } catch (const InsufficientGold &e) {
    game.sendSystemMessage(character.getId(), e.what());
  }
}

void Priest::listItems(Game &game, Character &character) {
  const auto &items = store.getItems();
  if (items.empty()) {
    game.sendSystemMessage(character.getId(),
                           "El sacerdote no tiene objetos disponibles.");
    return;
  }
  std::string msg = "Objetos del sacerdote: \n";
  for (auto &[id, item] : items) {
    msg += "    (# " + std::to_string(id) + ") " +
           ItemData::instance().getItemName(id) + " (x" +
           std::to_string(item.stock) +
           "). Precio de compra: " + std::to_string(item.purchase_price) +
           " \n";
  }
  msg.erase(msg.size() - 2);
  game.sendSystemMessage(character.getId(), msg);
}
