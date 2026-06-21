#ifndef INVENTORY_H
#define INVENTORY_H

#include <array>
#include <cstdint>
#include <memory>

#include "Armor.h"
#include "Helmet.h"
#include "InventoryConstants.h"
#include "Item.h"
#include "ItemData.h"

#include "Shield.h"
#include "Weapon.h"


class Inventory {
public:
  Inventory();

  bool addItem(uint8_t itemId);
  bool removeItem(uint8_t slotIndex);
  uint8_t findItem(uint8_t itemId) const;
  bool equipItem(uint8_t slotIndex);
  bool unequipSlot(EquipSlot slot);

  uint8_t getItemId(uint8_t slotIndex) const;
  size_t getItemCount() const;

  std::array<uint8_t, MAX_INVENTORY_SLOTS> getItems() const;
  void setItems(const std::array<uint8_t, MAX_INVENTORY_SLOTS> &newItems);

  const Weapon &getWeapon() const;
  const Armor &getArmor() const;
  const Helmet &getHelmet() const;
  const Shield &getShield() const;

  void setWeapon(uint8_t id);
  void setArmor(uint8_t id);
  void setHelmet(uint8_t id);
  void setShield(uint8_t id);

  void setSlotItemId(uint8_t slotIndex, uint8_t itemId);

private:
  std::array<std::unique_ptr<Item>, MAX_INVENTORY_SLOTS> items;
  Weapon equippedWeapon{0};
  Armor equippedArmor{0};
  Helmet equippedHelmet{0};
  Shield equippedShield{0};

  bool isSlotOccupied(EquipSlot slot) const;
  uint8_t equippedId(EquipSlot slot) const;
};

#endif
