#include "Inventory.h"
#include "EmptyItem.h"
#include "ItemData.h"

Inventory::Inventory() {
  for (auto &slot : items)
    slot = std::make_unique<EmptyItem>();
}

bool Inventory::addItem(uint8_t itemId) {
  if (itemId == 0)
    return false;
  for (auto &slot : items) {
    if (slot->isEmpty()) {
      slot = Item::create(itemId);
      return true;
    }
  }
  return false;
}

bool Inventory::removeItem(uint8_t slotIndex) {
  if (slotIndex >= MAX_INVENTORY_SLOTS || items[slotIndex]->isEmpty())
    return false;
  items[slotIndex] = std::make_unique<EmptyItem>();
  return true;
}

uint8_t Inventory::findItem(uint8_t itemId) const {
  for (uint8_t i = 0; i < MAX_INVENTORY_SLOTS; i++) {
    if (items[i]->getID() == itemId)
      return i;
  }
  return MAX_INVENTORY_SLOTS;
}

uint8_t Inventory::equippedId(EquipSlot slot) const {
  switch (slot) {
  case EquipSlot::Weapon:
    return equippedWeapon.getID();
  case EquipSlot::Armor:
    return equippedArmor.getID();
  case EquipSlot::Helmet:
    return equippedHelmet.getID();
  case EquipSlot::Shield:
    return equippedShield.getID();
  }
  return 0;
}

bool Inventory::equipItem(uint8_t slotIndex) {
  if (slotIndex >= MAX_INVENTORY_SLOTS || items[slotIndex]->isEmpty())
    return false;

  auto &idata = ItemData::instance();
  uint8_t newItemId = items[slotIndex]->getID();
  if (idata.isPotionHp(newItemId) || idata.isPotionMana(newItemId))
    return false;

  items[slotIndex]->switchInInventory(*this, slotIndex);
  return true;
}

bool Inventory::unequipSlot(EquipSlot slot) {
  uint8_t itemId = equippedId(slot);
  if (itemId == 0)
    return false;
  if (!addItem(itemId))
    return false;

  switch (slot) {
  case EquipSlot::Weapon:
    equippedWeapon = Weapon(0);
    break;
  case EquipSlot::Armor:
    equippedArmor = Armor(0);
    break;
  case EquipSlot::Helmet:
    equippedHelmet = Helmet(0);
    break;
  case EquipSlot::Shield:
    equippedShield = Shield(0);
    break;
  }
  return true;
}

uint8_t Inventory::getItemId(uint8_t slotIndex) const {
  if (slotIndex >= MAX_INVENTORY_SLOTS)
    return 0;
  return items[slotIndex]->getID();
}

size_t Inventory::getItemCount() const {
  size_t count = 0;
  for (auto &slot : items) {
    if (!slot->isEmpty())
      count++;
  }
  return count;
}

std::array<uint8_t, MAX_INVENTORY_SLOTS> Inventory::getItems() const {
  std::array<uint8_t, MAX_INVENTORY_SLOTS> ids{};
  for (size_t i = 0; i < MAX_INVENTORY_SLOTS; i++)
    ids[i] = items[i]->getID();
  return ids;
}

void Inventory::setItems(
    const std::array<uint8_t, MAX_INVENTORY_SLOTS> &newItems) {
  for (size_t i = 0; i < MAX_INVENTORY_SLOTS; i++)
    items[i] = Item::create(newItems[i]);
}

void Inventory::setSlotItemId(uint8_t slotIndex, uint8_t itemId) {
  if (slotIndex < MAX_INVENTORY_SLOTS)
    items[slotIndex] = Item::create(itemId);
}

bool Inventory::isSlotOccupied(EquipSlot slot) const {
  return equippedId(slot) != 0;
}
