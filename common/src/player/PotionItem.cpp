#include "PotionItem.h"
#include "Inventory.h"
#include "ItemData.h"

PotionItem::PotionItem(uint8_t id) : Item(id) {}

uint8_t PotionItem::getID() const { return id; }

void PotionItem::switchInInventory(Inventory &, uint8_t) {}

uint16_t PotionItem::healAmount() const {
  return ItemData::instance().getPotionData(id).healAmount;
}

const std::string &PotionItem::name() const {
  return ItemData::instance().getPotionData(id).name;
}
