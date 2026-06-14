#include "Armor.h"
#include "Inventory.h"
#include "ItemData.h"

uint16_t Armor::minDefense() const {
  return ItemData::instance().getArmorData(id).minDefense;
}

uint16_t Armor::maxDefense() const {
  return ItemData::instance().getArmorData(id).maxDefense;
}

const std::string &Armor::name() const {
  return ItemData::instance().getArmorData(id).name;
}

void Armor::switchInInventory(Inventory &inv, uint8_t slotIndex) {
  uint8_t oldId = inv.getArmor().getID();
  inv.setArmor(id);
  inv.setSlotItemId(slotIndex, oldId);
}
