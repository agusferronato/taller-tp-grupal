#include "Inventory.h"

Inventory::Inventory() { items.fill(EMPTY_ITEM); }

bool Inventory::addItem(uint8_t itemId) {
  if (itemId == EMPTY_ITEM || itemId >= MAX_ITEM_TYPES)
    return false;
  for (auto &slot : items) { 
    if (slot == EMPTY_ITEM) {
      slot = itemId;
      return true;
    }
  }
  return false;
}

bool Inventory::removeItem(uint8_t slotIndex) {
  if (slotIndex >= MAX_INVENTORY_SLOTS)
    return false;
  if (items[slotIndex] == EMPTY_ITEM)
    return false;

  items[slotIndex] = EMPTY_ITEM;
  return true;
}

uint8_t Inventory::findItem(uint8_t itemId) const {
  for (uint8_t i = 0; i < MAX_INVENTORY_SLOTS; i++) {
    if (items[i] == itemId)
      return i;
  }
  return MAX_INVENTORY_SLOTS;
}

bool Inventory::equipItem(uint8_t slotIndex) {
  if (slotIndex >= MAX_INVENTORY_SLOTS)
    return false;
  uint8_t newItemId = items[slotIndex];
  if (newItemId == EMPTY_ITEM)
    return false;

  const ItemDef &def = ITEM_TABLE[newItemId];

  // Pociones se consumen en InventoryManager, no se equipan.
  if (def.type == ItemType::PotionHp || def.type == ItemType::PotionMana) {
    return false;
  }

  EquipSlot eslot = slotForType(def.type);

  uint8_t *equippedVar = nullptr;
  switch (eslot) {
  case EquipSlot::Weapon:
    equippedVar = &equippedWeapon;
    break;
  case EquipSlot::Armor:
    equippedVar = &equippedArmor;
    break;
  case EquipSlot::Helmet:
    equippedVar = &equippedHelmet;
    break;
  case EquipSlot::Shield:
    equippedVar = &equippedShield;
    break;
  }

  if (!equippedVar)
    return false;

  // Realizar el intercambio (swap)
  uint8_t oldItemId = *equippedVar;
  *equippedVar = newItemId;
  items[slotIndex] = oldItemId;

  return true;
}

bool Inventory::unequipSlot(EquipSlot slot) {
  if (!isSlotOccupied(slot))
    return false;

  uint8_t itemId = (slot == EquipSlot::Weapon)   ? equippedWeapon
                   : (slot == EquipSlot::Armor)  ? equippedArmor
                   : (slot == EquipSlot::Helmet) ? equippedHelmet
                                                 : equippedShield;

  if (!addItem(itemId))
    return false;

  unequipSlotRaw(slot);
  return true;
}

uint8_t Inventory::getItemId(uint8_t slotIndex) const {
  if (slotIndex >= MAX_INVENTORY_SLOTS)
    return EMPTY_ITEM;
  return items[slotIndex];
}

size_t Inventory::getItemCount() const {
  size_t count = 0;
  for (auto &slot : items) {
    if (slot != EMPTY_ITEM)
      count++;
  }
  return count;
}

void Inventory::setItems(
    const std::array<uint8_t, MAX_INVENTORY_SLOTS> &newItems) {
  items = newItems;
}

EquipSlot Inventory::slotForType(ItemType type) const {
  switch (type) {
  case ItemType::Weapon:
  case ItemType::Staff:
    return EquipSlot::Weapon;
  case ItemType::Armor:
    return EquipSlot::Armor;
  case ItemType::Helmet:
    return EquipSlot::Helmet;
  case ItemType::Shield:
    return EquipSlot::Shield;
  default:
    return EquipSlot::Weapon;
  }
}

bool Inventory::isSlotOccupied(EquipSlot slot) const {
  switch (slot) {
  case EquipSlot::Weapon:
    return equippedWeapon != EMPTY_ITEM;
  case EquipSlot::Armor:
    return equippedArmor != EMPTY_ITEM;
  case EquipSlot::Helmet:
    return equippedHelmet != EMPTY_ITEM;
  case EquipSlot::Shield:
    return equippedShield != EMPTY_ITEM;
  }
  return false;
}

void Inventory::unequipSlotRaw(EquipSlot slot) {
  switch (slot) {
  case EquipSlot::Weapon:
    equippedWeapon = EMPTY_ITEM;
    break;
  case EquipSlot::Armor:
    equippedArmor = EMPTY_ITEM;
    break;
  case EquipSlot::Helmet:
    equippedHelmet = EMPTY_ITEM;
    break;
  case EquipSlot::Shield:
    equippedShield = EMPTY_ITEM;
    break;
  }
}
