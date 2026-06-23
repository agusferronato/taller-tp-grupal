#include "Helmet.h"
#include "Inventory.h"
#include "ItemData.h"

Helmet::Helmet(uint8_t id) : Item(id) {}

uint8_t Helmet::getID() const { return id; }

uint16_t Helmet::minDefense() const {
  return ItemData::instance().getHelmetData(id).minDefense;
}

uint16_t Helmet::maxDefense() const {
  return ItemData::instance().getHelmetData(id).maxDefense;
}

const std::string &Helmet::name() const {
  return ItemData::instance().getHelmetData(id).name;
}

void Helmet::switchInInventory(Inventory &inv, uint8_t slotIndex) {
  uint8_t oldId = inv.getHelmet().getID();
  inv.setHelmet(id);
  inv.setSlotItemId(slotIndex, oldId);
}
