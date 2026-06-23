#include "BankAccount.h"

void BankAccount::takeItem(uint8_t item) {
  auto it = items.find(item);
  if (it == items.end())
    throw ItemNotFoundInBank();

  it->second.amount--;

  if (!it->second.amount)
    items.erase(item);
}

void BankAccount::saveItem(uint8_t item) {
  auto it = items.find(item);
  if (it == items.end()) {
    auto result = items.insert({item, ItemInBank{}});
    it = result.first;
  }

  it->second.amount++;
}

void BankAccount::takeGold(uint16_t ammount) {
  if (ammount > gold)
    throw NoGoldAvailable();

  gold -= ammount;
}

void BankAccount::saveGold(uint16_t ammount) { gold += ammount; }

const std::map<uint8_t, ItemInBank> &BankAccount::getItems() const {
  return items;
}

uint16_t BankAccount::getGold() { return gold; }
