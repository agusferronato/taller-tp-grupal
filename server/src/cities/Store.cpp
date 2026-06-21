#include "Store.h"
#include "Character.h"

ItemInStore getStoreItem(uint8_t itemId) {
  const PriceData &price = ItemData::instance().getPriceData(itemId);
  return {price.buyPrice, price.sellPrice, 0};
}

Store::Store(std::map<uint8_t, ItemInStore> items,
             std::set<uint8_t> itemsNotAvailablesForTrading)
    : items(std::move(items)),
      itemsNotAvailablesForTrading(std::move(itemsNotAvailablesForTrading)) {}

const std::map<uint8_t, ItemInStore> &Store::getItems() const { return items; }

bool Store::isAvailableForTrading(uint8_t item) {
  auto it = itemsNotAvailablesForTrading.find(item);
  return it == itemsNotAvailablesForTrading.end();
}

void Store::buyItemWith(Character &character, uint8_t item) {
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

  character.spendGold(purchase_price);
}

void Store::sellItem(Character &character, uint8_t item) {
  if (!character.hasItem(item))
    throw PlayerDoesNotHaveTheItem();

  if (!isAvailableForTrading(item))
    throw ItemNotAvailable();

  auto it = items.find(item);
  if (it == items.end()) {
    auto result = items.insert({item, getStoreItem(item)});
    it = result.first;
  }

  it->second.stock++;

  character.removeItemById(item);
  character.addGold(it->second.sell_price);
}
