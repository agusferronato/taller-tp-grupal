#include "Shield.h"
#include "Inventory.h"
#include "ItemData.h"

uint16_t Shield::minDefense() const {
  return ItemData::instance().getShieldData(id).minDefense;
}

uint16_t Shield::maxDefense() const {
  return ItemData::instance().getShieldData(id).maxDefense;
}

const std::string &Shield::name() const {
  return ItemData::instance().getShieldData(id).name;
}

void Shield::switchInInventory(Inventory &inv, uint8_t slotIndex) {
  uint8_t oldId = inv.getShield().getID();
  inv.setShield(id);
  inv.setSlotItemId(slotIndex, oldId);
}
